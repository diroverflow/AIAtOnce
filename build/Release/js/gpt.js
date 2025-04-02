(() => {
  console.log('new gpt message');
  const %s;
  let inputField = document.querySelector('.ProseMirror');
  if(bFocus) {
	  inputField.focus();
	  if(bMsg) window.wx_msg.postMessage('focused');
  }
  if(bSent) {
	  inputField.innerHTML = `%s`;
	  inputField.dispatchEvent(new Event('input', { bubbles: true }));
	  setTimeout(()=>{inputField.dispatchEvent(new KeyboardEvent('keydown', {
		  key: 'Enter',
		  code: 'Enter',
		  keyCode: 13,
		  which: 13,
		  bubbles: true
	  }));}, 500);
	  window.wx_msg.postMessage('sent');
  }
})();