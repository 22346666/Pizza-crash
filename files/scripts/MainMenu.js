function getCookie(name) {
    var dc = document.cookie;
    var prefix = name + "=";
    var begin = dc.indexOf("; " + prefix);
    if (begin == -1) {
        begin = dc.indexOf(prefix);
        if (begin != 0) return null;
    }
    else
    {
        begin += 2;
        var end = document.cookie.indexOf(";", begin);
        if (end == -1) {
        end = dc.length;
        }
    }
    // because unescape has been deprecated, replaced with decodeURI
    //return unescape(dc.substring(begin + prefix.length, end));
    return decodeURI(dc.substring(begin + prefix.length, end));
} 

function CheckEmail()
{
	let CookEmail=getCookie("email");
	if(CookEmail==null)
	{
		document.getElementById("MenuModifiable").innerHTML=
			'<h1><a href="Login.html" class="YellTextMain">Log in</a></h1>\
			<h1><a href="Register.html" class="YellTextMain">Register</a></h1>';
	}
	else
	{
		document.getElementById("MenuModifiable").innerHTML=
			'<h1><a href="#" class="YellTextMain">Profile</a></h1>\
			<h1><a href="https://pizzacrash.servebeer.com/sign_in" class="YellTextMain" onclick="LogOut()">Log out</a></h1>';
	}
}

function LogOut()
{
	var CEm = document.cookie.split("; ").find((row) => row.startsWith("email="))?.split("=")[1];
  document.cookie = CEm + "=; expires="+ new Date(0).toUTCString();
  location.reload();
}
