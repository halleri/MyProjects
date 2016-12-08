
document.addEventListener('DOMContentLoaded', table);

function table() {
	
	document.getElementById('submit').addEventListener('click', function(event){
		
    var name = document.getElementById("name");
    var reps = document.getElementById("reps");
    var weight = document.getElementById("weight");
    var date = document.getElementById("date");
	 var lbs = document.getElementById("lbs");
	 var tableElem = document.getElementById("table");

//	var btn = document.createElement("BUTTON");
//	var btnText = document.createTextNode("Delete");
	var req = new XMLHttpRequest();

	var payload = {name:null, reps:null, weight:null, date:null, lbs:null};
	payload.name = document.getElementById('name').value;	
	payload.reps = document.getElementById('reps').value;
	payload.weight = document.getElementById('weight').value;
	payload.date = document.getElementById('date').value;
	payload.lbs = document.getElementById('lbs').value;

	req.open('GET', 'http://52.89.112.98:8080/insert?name='+payload.name +'&reps='+payload.reps+'&weight='+payload.weight +'&date='+payload.date+'&lbs='+payload.lbs, true);
	req.setRequestHeader("Content-type", "application/json");

	req.addEventListener('load', function(event){
		if(req.status >= 200 && req.status < 400) {
   
   			  var newRow = tableElem.insertRow(-1);
       			 var newCell = newRow.insertCell(0);
    		    var newText = document.createTextNode(name.value);
       		 newCell.appendChild(newText);
      		  newCell = newRow.insertCell(1);
       		 newText  = document.createTextNode(reps.value);
       		 newCell.appendChild(newText);
       		 newCell = newRow.insertCell(2);
       		 newText  = document.createTextNode(weight.value);
      		  newCell.appendChild(newText);
		
	        newCell = newRow.insertCell(3);
       		 newText  = document.createTextNode(date.value);
      		  newCell.appendChild(newText);
		
      		  newCell = newRow.insertCell(4);
       		 newText  = document.createTextNode(lbs.value);
       		 newCell.appendChild(newText);

//		newCell = newRow.insertCell();
//		btn.appendChild(btnText(;
//		newCell.appendChild(btn);

	/*	if(btn){
			deleteBtn();	
		}*/
			
		}
		else{
			console.log("Error in network " + request.status);
		}
	})
	req.send(JSON.stringify(payload));
	event.preventDefault();
})
}


document.addEventListener('DOMContentLoaded', deleteBtn);

function deleteBtn() {
	
	document.getElementById('delete').addEventListener('click', function(event){
		
    var id = document.getElementById("id");

	var req = new XMLHttpRequest();

	var payload = {id:null};
	payload.id = document.getElementById('id').value;	

	req.open('GET', 'http://52.89.112.98:8080/delete?id='+payload.id, true);
	req.setRequestHeader("Content-type", "application/json");

	req.addEventListener('load', function(event){
		if(req.status >= 200 && req.status < 400) {
   
			var row = document.getElementById('id');
			row.parentNode.removeChild(row);	
		}
		else{
			console.log("Error in network " + request.status);
		}
	})
	req.send(JSON.stringify(payload));
	event.preventDefault();
})
}




