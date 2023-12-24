var UName;
var USName;
var UEmail;

function ShowCredentials()
{
  UEmail=Cookies.get("email");
   var request = $.ajax({
                                 url: "https://pizzacrash.servebeer.com/get/user/"+UEmail,
                                 method: "GET",
                                 dataType: "json"
                             });
        request.done(function(data) {
            UName=data["Name"];
            USname=data["Surname"];

          document.getElementById("Credentials").innerHTML="<p style='‎YellTextSecondary'>Name: "+UName+"</p>"+
            "<p style='‎YellTextSecondary'>Surname: "+USName+"</p>"+
            "<p style='‎YellTextSecondary'>Email: "+UEmail+"</p>";
      
        });
        request.fail(function(jqXHR, textStatus) {
            console.error("Error: " + textStatus);
        });
}

function SendPasswordChange()
{
  var NewPass=document.getElementById("ProfNewPassword").vaulue;
  var ConfPass=document.getElementById("ProfConfPassword").vaulue;
  if(NewPass===ConfPass)
  {
      var OrderObj=new Object();
      OrderObj["Email"]=UEmail;
      OrderObj["Name"]=UName;
      OrderObj["Password"]=NewPass;
      OrderObj["Surname"]=USname;
      
      $.post("https://pizzacrash.servebeer.com/patch/user",
              JSON.stringify(OrderObj)
    );
  }
  else
  {
    window.alert("Passwords don't match!");
  }
}
