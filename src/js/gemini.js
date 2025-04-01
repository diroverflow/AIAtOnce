(() => {
  console.log('new gemini message');
  const %s;
  if(bFocus) {
	  let inputField = document.querySelector('textarea[aria-label="Type something"]');
	  inputField.focus();
	  inputField.click();
	  const intext = `%s`;
	  const nativeSetter = Object.getOwnPropertyDescriptor(window.HTMLTextAreaElement.prototype, 'value').set;
	  nativeSetter.call(inputField, intext);
	  inputField.dispatchEvent(new Event('input', { bubbles: true }));
	  const submitButton = document.querySelector('button[type="submit"]');
		  if (submitButton) {
			submitButton.click();
		  } else {
			console.error('no button found');
		  }
	  inputField.dispatchEvent(new KeyboardEvent('keydown', {
		  key: 'Enter',
		  code: 'Enter',
		  ctrlKey: true,
		  bubbles: true,
		  cancelable: true
		})); 
	  inputField.dispatchEvent(new KeyboardEvent('keyup', {
		  key: 'Enter',
		  code: 'Enter',
		  ctrlKey: false,
		  bubbles: true,
		  cancelable: true
		})); 
	  if(bMsg) window.wx_msg.postMessage('focused');
  }
  if(bSent) {
	  if(window.__MUTATION_GEMINI_OBSERVERS__ === 1) {
		console.log('Mutation observer already exists');
		return;
	  } else {
		const targetNode = document.querySelector('ms-chat-session');//ms-autoscroll-container.ng-star-inserted[tabindex="-1"]');
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