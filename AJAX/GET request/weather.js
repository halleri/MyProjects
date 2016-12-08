var apiKeyZipCode = 'a60ffdc1de0ee548ef8eb5721c15dd2e';
document.addEventListener('DOMContentLoaded', bindButtons);

/*Some of this code is courtesy of the code from the week 6 lectures*/

function bindButtons(){
//When this function is called, the submit button is 'linked' to
//this function. So, whatever happens inside this function gets called
//when the submit button is pressed.
	document.getElementById('urlSubmit').addEventListener('click', function(event){
		//assign a request to a variable. this either gets or sends data.
		var req = new XMLHttpRequest();
		//create an object and set it to the value, weather api is expecting
		var payload = {zip:null};
		payload.zip = document.getElementById('zip').value;
		//a get requests gathers information
		req.open('GET', 'http://api.openweathermap.org/data/2.5/weather?zip='+payload.zip+',us&appid=a60ffdc1de0ee548ef8eb5721c15dd2e', true);
		req.addEventListener('load', function(){
			if(req.status >= 200 && req.status < 400) {
				var response = JSON.parse(req.responseText);
				document.getElementById('city').textContent = response.name;
				document.getElementById('temp').textContent = response.main.temp;
				document.getElementById('humidity').textContent = response.main.humidity;
			}else{
				console.log("Error in network request: " + request.status);
			}
		});
		req.send(JSON.stringify(payload));
		event.preventDefault();	
	})
}