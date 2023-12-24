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
            USName=data["Surname"];

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
  var NewPass=document.getElementById("ProfNewPassword").value;
  var ConfPass=document.getElementById("ProfConfPassword").value;
  if((NewPass!=="")&&(NewPass!=null)&&(ConfPass!=="")&&(ConfPass!=null))
  {
  if(NewPass===ConfPass)
  {
      var OrderObj=new Object();
      OrderObj["Email"]=UEmail;
      OrderObj["Name"]=UName;
      OrderObj["Password"]=NewPass;
      OrderObj["Surname"]=USName;
      
      $.post("https://pizzacrash.servebeer.com/patch/user",
              JSON.stringify(OrderObj)
    );
  }
  else
  {
    window.alert("Passwords don't match!");
  }
  }
  else
  {window.alert("Please enter all of the fields!")}
}
