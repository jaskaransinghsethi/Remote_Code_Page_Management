function showHideClass(className) {
	let elems = document.getElementsByClassName(className);
	if (elems === null) {
		return;
	}
	for (let i = 0; i < elems.length; ++i) {
		let displayProp = elems[i].style.display;
	if (displayProp === 'none')
		elems[i].style['display'] = "inline";
	else
		elems[i].style['display'] = "none";
	}
}

function showHideALL() {
	var elems1 = document.getElementsByClassName("comments");
	var elems2 = document.getElementsByClassName("classes");
	var elems3 = document.getElementsByClassName("functions");
	if (elems1 === null) {
		return;
	}
	for (let i = 0; i < elems1.length; ++i) {
		let displayProp = elems1[i].style.display;
		if (displayProp === 'none')
			elems1[i].style['display'] = "inline";
		else
			elems1[i].style['display'] = "none";
	}
	if (elems2 === null) {
		return;
	}
	for (let i = 0; i < elems2.length; ++i) {
		let displayProp = elems2[i].style.display;
		if (displayProp === 'none')
			elems2[i].style['display'] = "inline";
		else
			elems2[i].style['display'] = "none";
	}
	if (elems3 === null) {
		return;
	}
	for (let i = 0; i < elems3.length; ++i) {
		let displayProp = elems3[i].style.display;
		if (displayProp === 'none')
			elems3[i].style['display'] = "inline";
		else
			elems3[i].style['display'] = "none";
	}
	
}


