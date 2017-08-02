
'use strict';

//the program works when this is commented
/*(function(){
  var config = {
    apiKey: "AIzaSyCAkuwk-kintiq4KJKsX0_oly25bUG2eck",
    authDomain: "real-time-chat-5dea5.firebaseapp.com",
    databaseURL: "https://real-time-chat-5dea5.firebaseio.com",
    projectId: "real-time-chat-5dea5",
    storageBucket: "real-time-chat-5dea5.appspot.com",
    messagingSenderId: "198818296118"
  };
  firebase.initializeApp(config);

}());*/

function Chat(){
  this.messageForm = document.getElementById('message-form');
  this.signInButton = document.getElementById('sign-in');
  this.signOutButton = document.getElementById('sign-out');

  this.messageForm.onclick = ('submit', this.saveMessage.bind(this));

  this.signOutButton.onclick = ('sign-out', this.signOut.bind(this));
  this.signInButton.onclick = ('sign-in', this.signIn.bind(this));

  this.auth = firebase.auth();
  this.database = firebase.database();
  this.auth.onAuthStateChanged(this.onAuthStateChanged.bind(this));

}


function load() {
  window = new Chat();
}

// loads messages
Chat.prototype.loadMessages = function() {

  var setMessage = function(data) {
    var val = data.val();
    this.showMessage(data.key, val.name, val.text);
  }.bind(this);
this.database.ref('messages').limitToLast(5).on('child_added',  setMessage);
this.database.ref('messages').limitToLast(5).on('child_changed', setMessage);
};

// sends and saves message to firebase db
Chat.prototype.saveMessage = function(e) {
  e.preventDefault();
  if (document.getElementById('message').value && this.auth.currentUser) {
    //send new message to database
    this.database.ref('messages').push({
      name: this.auth.currentUser.displayName,
      text: document.getElementById('message').value,
    }).then(function() {
      // clear message in text field
      document.getElementById('message').value = '';
    }.bind(this)).catch(function(error) {
      console.error(error);
    });
  }
};

// Sign in
Chat.prototype.signIn = function() {
  this.auth.signInWithPopup(new firebase.auth.GoogleAuthProvider());
};

// Sign out
Chat.prototype.signOut = function() {
  this.auth.signOut();
};


//https://firebase.google.com/docs/reference/js/firebase.auth.Auth#onAuthStateChanged

// state change between sign in and sign out notification
Chat.prototype.onAuthStateChanged = function(user) {
  if (user) {
    this.signOutButton.removeAttribute('hidden');
    // Hide sign-in button.
    this.signInButton.setAttribute('hidden', 'true');

    this.loadMessages();

  } else {
    this.signOutButton.setAttribute('hidden', 'true');
    // Show sign-in button.
    this.signInButton.removeAttribute('hidden');
  }
};

// shows message
Chat.prototype.showMessage = function(key, name, text) {

  var div = document.getElementById(key);
  if (!div) {
    var container = document.createElement('div');
    container.innerHTML = '<div class="chatlog">' +
                          '<div class="message"></div>' +
                          '<div class="name"></div>' +
                          '</div>';
    div = container.firstChild;
    document.getElementById('messages').appendChild(div);
  }
  div.querySelector('.name').textContent = name;
  var messageElement = div.querySelector('.message');
    messageElement.textContent = text;
  setTimeout(function() {
    div.classList.add('visible')}, 1);
};
