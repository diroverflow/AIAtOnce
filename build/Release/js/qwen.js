(() => {
  console.log('new qwen message');
  const %s;
  let inputField = document.querySelector('#chat-input');
  if(bFocus) {
	  inputField.focus();
	  inputField.click();
	  const intext = `%s`;
		const nativeSetter = Object.getOwnPropertyDescriptor(window.HTMLTextAreaElement.prototype, 'value').set;
		nativeSetter.call(inputField, intext);
		inputField.dispatchEvent(new Event('input', { bubbles: true }));
		const form = document.querySelector('form');
		const disabledElements = form.querySelectorAll('[disabled]');
		disabledElements.forEach(element => {
		element.removeAttribute('disabled');
		element.disabled = false;
		});
		// const submitDescriptor = Object.getOwnPropertyDescriptor(HTMLFormElement.prototype, "submit");
		// if (submitDescriptor && typeof submitDescriptor.value === "function") {
		  // submitDescriptor.value.call(form);
		// }
		
	  // const submitButton = form.querySelector('button[type="submit"]');
		  // if (submitButton) {
			// submitButton.click();
 		  // } else {
			// console.error('no button found');
			// form.submit();
		   // }
	  if(bMsg) window.wx_msg.postMessage('focused');
  }
  if(bSent) {
	  if(window.__MUTATION_GEMINI_OBSERVERS__ === 1) {
		console.log('Mutation observer already exists');
		return;
	  } else {
		//const targetDiv = document.querySelector('div.ds-markdown.ds-markdown--block');
		//if (targetDiv) {
		  const targetNode = document.querySelector('div[data-pane]');
			console.log(targetNode);
			if (!targetNode) {
				console.error('no ms-autoscroll-container');
			} else {
			  console.log('Mutation observer created');
			  setTimeout(() => {
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
			}, 2000);
		}
	}
  }
})();