(() => {
  console.log('new doubao message');
  const %s;
  let inputField = document.querySelector('textarea[data-testid="chat_input_input"]');
  if(bFocus) {
	  inputField.focus();
	  inputField.click();
	  const intext = `%s`;
		const nativeSetter = Object.getOwnPropertyDescriptor(window.HTMLTextAreaElement.prototype, 'value').set;
		nativeSetter.call(inputField, intext);
		inputField.dispatchEvent(new Event('input', { bubbles: true }));
	  const submitButton = document.querySelector('button[id="flow-end-msg-send"]');
		  if (submitButton) {
			  submitButton.setAttribute('aria-disabled','false');
			  submitButton.disabled = false;
			  submitButton.removeAttribute('disabled');
			submitButton.click();
		  } else {
			console.error('no button found');
		  }
	  if(bMsg) window.wx_msg.postMessage('focused');
  }
  if(bSent) {
	  if(window.__MUTATION_GEMINI_OBSERVERS__ === 1) {
		console.log('Mutation observer already exists');
		return;
	  } else {
		  setTimeout(() => {

		  const targetNode = document.querySelector('div[class^="message-list-"]');
			console.log(targetNode);
			if (!targetNode) {
				console.error('no ms-autoscroll-container');
			} else {
			  console.log('Mutation observer created');
				const observer = new MutationObserver((mutationsList, observer) => {
					for (const mutation of mutationsList) {
						if (mutation.type === 'childList' || mutation.type === 'subtree') {
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
		}, 1500);
	  }
  }
})();