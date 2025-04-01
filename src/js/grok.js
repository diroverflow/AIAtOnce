(() => {
		//debugger;
    console.log('new grok message');
	const %s;
	let inputField = document.querySelector('textarea[dir="auto"]');
	if(bFocus) {
		inputField.focus();
		inputField.click();
		const intext = `%s`;
		const nativeSetter = Object.getOwnPropertyDescriptor(window.HTMLTextAreaElement.prototype, 'value').set;
		nativeSetter.call(inputField, intext);
		inputField.dispatchEvent(new Event('input', { bubbles: true }));
		const div = document.querySelector('div.bg-button-secondary.text-secondary.ring-0');
		if (div) {
		  div.className = 'h-9 relative aspect-square flex flex-col items-center justify-center rounded-full ring-1 ring-inset before:absolute before:inset-0 before:rounded-full before:bg-primary before:ring-0 before:transition-all duration-500 bg-primary text-background ring-transparent before:[clip-path:circle(50%%_at_50%%_50%%)]';
		} else {
		  console.error('no div found');
		}
		const form = document.querySelector('form');
		const disabledElements = form.querySelectorAll('[disabled]');
		  disabledElements.forEach(element => {
			element.removeAttribute('disabled');
			element.disabled = false;
		  });
		const submitButton = form.querySelector('button[type="submit"]');
		  if (submitButton) {
			submitButton.click();
		  } else {
			console.error('no button found');
			form.submit();
		  }
		if(bMsg) window.wx_msg.postMessage('focused');
	}
	if(bSent) {
		if(window.__MUTATION_GEMINI_OBSERVERS__ === 1) {
		console.log('Mutation observer already exists');
		return;
	  } else {
		const targetNode = document.querySelector('main');
		if (!targetNode) {
			console.error('no ms-autoscroll-container');
		} else {
		  console.log('Mutation observer created');
			const observer = new MutationObserver((mutationsList, observer) => {
				for (const mutation of mutationsList) {
					if (mutation.type === 'childList') {
						console.log('subtree changed');
						console.log('add:', mutation.addedNodes);
						console.log('del:', mutation.removedNodes);
						window.wx_msg.postMessage('sent');
					}
				}
			});
			observer.observe(targetNode, {childList: true, subtree: true});
			window.__MUTATION_GEMINI_OBSERVERS__ = 1;
		}
	  }
	}
  })();