/////////////////////////////////////////////////////////////////////////
// ServerPrototype.cpp - Console App that processes incoming messages  //
// ver 1.3                                                             //
// Jim Fawcett, CSE687 - Object Oriented Design, Spring 2018           //
/////////////////////////////////////////////////////////////////////////

#include "Server.h"
#include "../Process/Process.h"
#include <chrono>
#include <future>

	namespace MsgPassComm = MsgPassingCommunication;

	using namespace Repository;
	using namespace FileSystem;
	using Msg = MsgPassingCommunication::Message;

	//----< return name of every file on path >----------------------------

	Files Server::getFiles(const Repository::SearchPath& path)
	{
		return Directory::getFiles(path);
	}
	//----< return name of every subdirectory on path >--------------------

	Dirs Server::getDirs(const Repository::SearchPath& path)
	{
		return Directory::getDirectories(path);
	}

	namespace MsgPassingCommunication
	{
		// These paths, global to MsgPassingCommunication, are needed by 
		// several of the ServerProcs, below.
		// - should make them const and make copies for ServerProc usage

		std::string sendFilePath;
		std::string saveFilePath;

		//----< show message contents >--------------------------------------

		template<typename T>
		void show(const T& t, const std::string& msg)
		{
			std::cout << "\n  " << msg.c_str();
			for (auto item : t)
			{
				std::cout << "\n    " << item.c_str();
			}
		}

		//----< test ServerProc simply echos message back to sender >--------
		Repository::ServerProc echo = [](Msg msg, std::function<void(Msg)> postMsg) {
			Msg reply = msg;
			reply.to(msg.from());
			reply.from(msg.to());
			postMsg(reply);
		};

		//----< getFiles ServerProc returns list of files on path >----------
		Repository::ServerProc getFiles = [](Msg msg, std::function<void(Msg)> postMsg) {
			Msg reply;
			reply.to(msg.from());
			reply.from(msg.to());
			reply.command("getFiles");
			std::string path = msg.value("path");
			if (path != "")
			{
				std::string searchPath = storageRoot;
				if (path != ".")
					searchPath = searchPath + "\\" + path;
				Files files = Server::getFiles(searchPath);
				size_t count = 0;
				for (auto item : files)
				{
					std::string countStr = Utilities::Converter<size_t>::toString(++count);
					reply.attribute("file" + countStr, item);
				}
			}
			else
			{
				std::cout << "\n  getFiles message did not define a path attribute";
			}
			postMsg(reply);
		};

		//----< getDirs ServerProc returns list of directories on path >-----
		Repository::ServerProc getDirs = [](Msg msg, std::function<void(Msg)> postMsg) {
			Msg reply;
			reply.to(msg.from());
			reply.from(msg.to());
			reply.command("getDirs");
			std::string path = msg.value("path");
			if (path != "")
			{
				std::string searchPath = storageRoot;
				if (path != ".")
					searchPath = searchPath + "\\" + path;
				Files dirs = Server::getDirs(searchPath);
				size_t count = 0;
				for (auto item : dirs)
				{
					if (item != ".." && item != ".")
					{
						std::string countStr = Utilities::Converter<size_t>::toString(++count);
						reply.attribute("dir" + countStr, item);
					}
				}
			}
			else
			{
				std::cout << "\n  getDirs message did not define a path attribute";
			}
			postMsg(reply);
		};


		//----< sendFile ServerProc sends file to requester >----------------
		/*
		*  - Comm sends bodies of messages with sendingFile attribute >------
		*/
		Repository::ServerProc sendFile = [](Msg msg, std::function<void(Msg)> postMsg) {
			Msg reply;
			reply.to(msg.from());
			reply.from(msg.to());
			reply.command("sendFile");
			reply.attribute("sendingFile", msg.value("fileName"));
			reply.attribute("fileName", msg.value("fileName"));
			if (msg.attributes()["convertedFile"] == "1")
				reply.attribute("convertedFile", "1");
			std::string path = msg.value("path");
			if (path != "")
			{
				std::string searchPath = storageRoot;
				if (path != "." && path != searchPath)
					searchPath = searchPath + "\\" + path;
				if (!FileSystem::Directory::exists(searchPath))
				{
					std::cout << "\n  file source path does not exist";
					postMsg(reply);
				}
				std::string filePath = searchPath + "/" + msg.value("fileName");
				std::string fullSrcPath = FileSystem::Path::getFullFileSpec(filePath);
				std::string fullDstPath = sendFilePath;
				if (!FileSystem::Directory::exists(fullDstPath))
				{
					std::cout << "\n  file destination path does not exist";
					postMsg(reply);
				}
				fullDstPath += "/" + msg.value("fileName");
				FileSystem::File::copy(fullSrcPath, fullDstPath);
			}
			else
			{
				std::cout << "\n  getDirs message did not define a path attribute";
			}
			postMsg(reply);
		};

		//----< saveFile ServerProc saves file on to the server directory >----------------
		
		Repository::ServerProc saveFiles = [](Msg msg, std::function<void(Msg)> postMsg) {
			Msg reply;
			reply.to(msg.from());
			reply.from(msg.to());
			reply.command("saveFiles");

			std::string path = msg.value("path");
			if (path != "")
			{
				std::string searchPath = storageRoot + "/" + FileSystem::Path::getName(path);
				if (!FileSystem::Directory::exists(searchPath))
				{
					FileSystem::Directory::createOnPath(searchPath);
				}
				std::string fullSrcPath = path;
				std::string fullDstPath = FileSystem::Path::getFullFileSpec(searchPath);
				if (!FileSystem::Directory::exists(fullDstPath))
				{
					std::cout << "\n  file destination path does not exist";
					reply.attributes()["Output"] = "Files failed to upload";
					postMsg(reply);
				}
				std::vector<std::string> pattern = Utilities::split(msg.value("patterns"), ' ');

				DirExplorerN de(path);
				for (auto patt : pattern)
				{
					de.addPattern(patt);
				}

				if (msg.value("options") == "/s")
				{
					de.recurse();
				}

				de.search();

				std::vector<std::string> files = de.fileToProcess();
				for (auto file : files) {
					std::string fileName =  "\\" + FileSystem::Path::getName(file);
					FileSystem::File::copy(file, fullDstPath + fileName);
				}
				reply.attributes()["Output"] = "Files successfully uploaded";
			}
			else
			{
				std::cout << "\n  getDirs message did not define a path attribute";
				reply.attributes()["Output"] = "Files failed to upload";
			}
			postMsg(reply);
		};

		//Helper function to handle code publish request
		void callPublisher(Msg msg, std::function<void(Msg)> postMsg) {
			Msg reply;
			int argc = 0;
			reply.to(msg.from());
			reply.from(msg.to());
			reply.command("codePublish");
			std::string path = msg.value("path");
			std::vector<std::string> args;
			args.push_back("USELESS");
			if (path != "") {
				std::string searchPath = storageRoot;
				if (path == "Storage")
				{
				}
				else
					if (path != "." && path != searchPath)
						searchPath = searchPath + "/" + FileSystem::Path::getName(path);
				if (!FileSystem::Directory::exists(searchPath))
				{
					std::cout << "\n  file source path does not exist";
					reply.attributes()["Output"] = "Files failed to convert";
					postMsg(reply);
				}
				args.push_back(searchPath);
			}
			std::string patterns = msg.value("patterns");
			if (patterns != "") {
				std::vector<std::string> pattern = Utilities::split(msg.value("patterns"), ' ');
				for (auto patt : pattern)
					args.push_back(patt);
			}
			std::string regex = msg.value("regex");
			if (regex != "")
				args.push_back(regex);
			std::string options = msg.value("options");
			if (options != "")
				args.push_back(options);
			argc += args.size();
			char** argv = new char*[argc];
			for (int i = 0; i < argc; i++) {
				std::string arg = args[i];
				argv[i] = new char[arg.length()]();
				strcpy_s(argv[i], arg.length() + 1, arg.c_str());
			}
			ObjectFactory factory;
			Ipublisher* client = factory.createClient();
			std::vector<std::string> files = client->startProject(argc, argv);
			int i = 0;
			for (auto file : files) {
				reply.attribute("file" + std::to_string(++i), file);
			}
			reply.attributes()["Output"] = "Files successfully converted";
			postMsg(reply);
		}

		//Dispatcher to handle codePublish request
		Repository::ServerProc codePublish = [](Msg msg, std::function<void(Msg)> postMsg) {
			std::thread workerThread(callPublisher, msg, postMsg);
			workerThread.detach();
		};
	}
	
	using namespace MsgPassingCommunication;

	int main()
	{
		SetConsoleTitleA("Server Console");

		std::cout << "\n  Server Initialising";
		std::cout << "\n ====================";
		std::cout << "\n";

		sendFilePath = FileSystem::Directory::createOnPath("./SendFiles");
		saveFilePath = FileSystem::Directory::createOnPath("./SaveFiles");

		Server server(serverEndPoint, "Server");

		// may decide to remove Context
		MsgPassingCommunication::Context* pCtx = server.getContext();
		pCtx->saveFilePath = saveFilePath;
		pCtx->sendFilePath = sendFilePath;

		server.start();

		server.addMsgProc("echo", echo);
		server.addMsgProc("getFiles", getFiles);
		server.addMsgProc("getDirs", getDirs);
		server.addMsgProc("sendFile", sendFile);
		server.addMsgProc("saveFiles", saveFiles);
		server.addMsgProc("codePublish", codePublish);
		server.addMsgProc("serverQuit", echo);

		server.processMessages();

		Msg msg(serverEndPoint, serverEndPoint);  // send to self
		msg.name("msgToSelf");

		std::cout << "\n  press enter to exit\n";
		std::cin.get();
		std::cout << "\n";

		msg.command("serverQuit");
		server.postMessage(msg);
		server.stop();
		return 0;
	}
