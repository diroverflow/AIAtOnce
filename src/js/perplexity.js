(() => {
  console.log('new perplexity message');
  const %s;
  let inputField = document.querySelector('textarea[data-1p-ignore="true"]');
  if(bFocus) {
	  inputField.focus();
	  inputField.click();
	  const intext = `%s`;
	  const nativeSetter = Object.getOwnPropertyDescriptor(window.HTMLTextAreaElement.prototype, 'value').set;
	  nativeSetter.call(inputField, intext);
	  inputField.dispatchEvent(new Event('input', { bubbles: true }));
	  
	  inputField.dispatchEvent(new KeyboardEvent('keydown', {
		  key: 'Enter',
		  code: 'Enter',
		  keyCode: 13,
		  which: 13,
		  bubbles: true
	  }));
	  inputField.dispatchEvent(new KeyboardEvent('keyup', {
		  key: 'Enter',
		  code: 'Enter',
		  keyCode: 13,
		  which: 13,
		  bubbles: true
	  }));
	  if(bMsg) window.wx_msg.postMessage('focused');
  }
  if(bSent) {
	  /*if(window.__MUTATION_GEMINI_OBSERVERS__ === 1) {
		console.log('Mutation observer already exists');
		return;
	  } else {*/
		const targetNode = document.querySelector('div.scrollable-container');
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
						observer.disconnect();
					}
				}
			});
			observer.observe(targetNode, {childList: true, subtree: true});
			//window.__MUTATION_GEMINI_OBSERVERS__ = 1;
		}
	  //}
  }
})();