(() => {
  console.log('new yiyan message');
  const %s;
  let inputField = document.querySelector('p.yc-editor-paragraph');
  if(bFocus) {
	  inputField.focus();
	  inputField.click();
	  if(bMsg) window.wx_msg.postMessage('focused');
  }
  if(bSent) {
	  if(window.__MUTATION_GEMINI_OBSERVERS__ === 1) {
		console.log('Mutation observer already exists');
		return;
	  } else {
		//const targetDiv = document.querySelector('div.ds-markdown.ds-markdown--block');
		//if (targetDiv) {
		  const targetNode = document.querySelector('#chat_container');
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
		//  }
	}
  }
})();