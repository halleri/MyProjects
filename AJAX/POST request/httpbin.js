document.addEventListener('DOMContentLoaded', bindButtons);
/*Some of this code is courtesy of the code from the week 6 lectures*/

function bindButtons(){
	//When this function is called, the submit button is 'linked' to
	//this function. So, whatever happens inside this function gets called
	//when the submit button is pressed.
	document.getElementById('urlSubmit').addEventListener('click', function(event){
		//assign a request to a variable. this either gets or sends data.
		var req = new XMLHttpRequest();
		//create an object and set it to the value, httpbin is expecting
		var payload = {data:null};
		//assign the 'value' to it
		payload.data = document.getElementById('data').value;
		//a post request creates data. we ask a user to input something
		//and then it's printed to the screen
		req.open('POST', "http://httpbin.org/post", true);
		req.setRequestHeader('Content-Type', 'application/json');
		req.addEventListener('load', function(){
			//if there is no timeout, then do the following
			if(req.status >= 200 && req.status < 400) {
				//parses the value from the key
				var response = JSON.parse(req.responseText);
				//parse the key from the value one more time
				document.getElementById('inputData').textContent = JSON.parse(response.data).data;
			}else{
				console.log("Error in network request: " + request.status);
			}
		});
		//convert JSON object to string
		req.send(JSON.stringify(payload));				
		event.preventDefault();	
	})
}