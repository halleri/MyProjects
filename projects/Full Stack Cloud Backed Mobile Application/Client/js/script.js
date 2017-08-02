$( document ).ready(function() {
  
  sessionStorage.setItem('public', 0);

  if(sessionStorage.getItem('pId') === null){
    $.ajax({
      type: "GET",
      url: 'http://web.engr.oregonstate.edu/~halleri/CS496/final/login.html',

      success:function(html) {
        $('#content').html(html);

      },
      error: function(html){
        alert("Error with login " + html.statusText);
      }
    });
  }
  else{
    $('#user').append("<h4>Welcome, "+sessionStorage.getItem('name')+"! <button onClick='logout()'>Logout</button></h4>");
    loadPage();
  }
});

function loadPage(){
  $.ajax({
    type: "GET",
    url: 'http://web.engr.oregonstate.edu/~halleri/CS496/final/books.html',

    success:function(html) {
      $('#content').html(html);
   

    },
    error: function(html){
      alert("Error loading page " + html.stat);
    }
  });


  pId = sessionStorage.getItem('pId')

  $.ajax({
    type: "GET",
    url: 'http://cs496final-140118.appspot.com/book',
    dataType: 'json',
    data: {pId: pId},

    success:function(html) {
      $('#books').empty();
      for (var x=0; x < html.length; x++){
        var bookhtml = html[x].title;

          bookhtml += "<button class='btn' onClick='edit("+html[x].key+")'>Edit/Delete</button>";
   
  
        bookhtml+="</div>";


      $('#books').append(bookhtml);
    }

    },
    error: function(html){
      alert("Request failed. Couldn't load book reviews. Sorry. Try again maybe?");
    }
  });
}

//attempts to log in / create new user
function login(){

  var name = $("input[name='name']").val();


  var pw = $("input[name='pw']").val();


  $.ajax({
    type: "POST",
    url: 'http://cs496final-140118.appspot.com/login',
    dataType: 'json',
    data:{
      pw: pw,
      name: name
    },
    success:function(html) {
      sessionStorage.setItem('pId', html.pId);
      sessionStorage.setItem('name', html.name);
      $('#user').append("<h3>Welcome, "+sessionStorage.getItem('name')+"! <button onClick='logout()'>Logout</button></h3>");
      loadPage();

    },
    error: function(html){
      alert("Request failed. " + html.statusText);
      console.log(html);
    }
  });
}


function to_add(){

var title = $("input[name='title']").val();


  var pId = sessionStorage.getItem('pId');

  var key = $("input[name='key']").val();

  var data = {
    title: title,
    pId: pId,
  }

      $.ajax({
        type: "POST",
        url: 'http://cs496final-140118.appspot.com/book',
        dataType: 'json',
        data: data,
        success:function(html) {
          loadPage();

        },
        error: function(html){
          alert("Book add fail " + html.statusText);
        }
      });


}


//populates form to change an entry
function edit(key){

  pId = sessionStorage.getItem('pId');

  $.ajax({
    type: "GET",
    url: 'http://cs496final-140118.appspot.com/book/' + key,
    dataType: 'json',
    data: {pId:pId},
    success:function(html) {
      $("input[name='key']").val(key);
      $("input[name='title']").val(html['title']);

      $("#addButton").text("Update entry");
      $("#deleteButton").removeClass('hidden').attr('onClick', 'to_delete('+key+')');

    },
    error: function(html){
      alert("Error editing " + html.statusText);
    }
  });


}


//deletes an entry
function to_delete(key){

  pId = sessionStorage.getItem('pId');

  $.ajax({
    type: "DELETE",
    url: 'http://cs496final-140118.appspot.com/book/' + key+'/'+pId,
    dataType: 'json',

    success:function(html) {
      $("#addButton").text("Delete");
      $("#deleteButton").addClass('hidden').attr('onClick', '');
      loadPage();

    },
    error: function(html){
      alert("Fail on delete " + html.statusText);
    }
  });



}

function logout(){
  sessionStorage.clear();
  $.ajax({
      type: "GET",
      url: 'http://web.engr.oregonstate.edu/~halleri/CS496/final/login.html',

      success:function(html) {
        $('#content').html(html);
        $('#books').empty();
        $('#user').empty();

      },
      error: function(html){
        alert("Error on logout " + html.statusText);
      }
    });
}




