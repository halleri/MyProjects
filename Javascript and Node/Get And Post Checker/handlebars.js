var express = require('express');

var app = express();
var handlebars = require('express-handlebars').create({defaultLayout:'main'});
var bodyParser = require('body-parser');

app.use(bodyParser.urlencoded({ extended: false }));
app.use(bodyParser.json());

app.engine('handlebars', handlebars.engine);
app.set('view engine', 'handlebars');
app.set('port', 3000);

app.get('/',function(req,res){
  res.render('home') 
});


//.url prints out each character individually

/*source: https://github.com/wolfordj/CS290-Server-Side-Examples/blob/master/express-forms/forms-demo.js */

/*This is the GET portion. It takes in a request and sends out a response.  */
app.get('/get',function(request,response){
  //An array is created. This will be used later in this function.
  var qParams = [];
  //Loop through each key/value in the request query. Query is used when dealing with GET requests. 
  for (p in request.query){
	  //push the name and value of the request into the array. 
    qParams.push({'name':p,'value':request.query[p]})
  }
  //Create an object.
  var context = {};
  //assign the array to the object above and give it a property. This property can be any name, but
  //it is going to be used in the get.handlebars file. 
  context.data = qParams;
  //this calls the get.handlebars file. It goes to that file and sees what it wants to do. Which is loop
  //through the key/value pairs and print them out accordingly. 
  response.render('get', context);
});

/*source: https://github.com/wolfordj/CS290-Server-Side-Examples/blob/master/express-forms/forms-demo.js */

/* This code is almost exactly the same as the 'get' function above. The only difference is this one 
depends on the 'body parser' which is included above. Also the request is put in the 'body' of the webpage.
POSTs can have data sent via URL; however, when you submit a POST request the response will most likely
be in the body somewhere. Because the assignment says the url should be the same /get is being used. This
directs the page to the /get page. */


app.post('/get', function(request,response){
  var qParams = [];
  for (p in request.body){
    qParams.push({'name':p,'value':request.body[p]})
  }
  var context = {};
  context.data = qParams;
  response.render('post', context);
});


app.use(function(req,res){
  res.status(404);
  res.render('404');
});

app.use(function(err, req, res, next){
  console.error(err.stack);
  res.type('plain/text');
  res.status(500);
  res.render('500');
});

app.listen(app.get('port'), function(){
  console.log('Express started on http://localhost:' + app.get('port') + '; press Ctrl-C to terminate.');
});