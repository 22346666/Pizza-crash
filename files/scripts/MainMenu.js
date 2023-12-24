//import Cookies from 'js-cookie'
/*
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
*/
function CheckEmail()
{
    let CookEmail=Cookies.get("email");
    if(CookEmail===undefined)
    {
        document.getElementById("MenuModifiable").innerHTML=
                '<h1><a href="https://pizzacrash.servebeer.com/sign_in" class="YellTextMain">Log in</a></h1>\
            <h1><a href="https://pizzacrash.servebeer.com/sign_up" class="YellTextMain">Register</a></h1>';
    }
    else
    {
        document.getElementById("MenuModifiable").innerHTML=
                '<h1><a href="https://pizzacrash.servebeer.com/profile" class="YellTextMain">Profile</a></h1>\
            <h1><a href="https://pizzacrash.servebeer.com/sign_in" class="YellTextMain" onclick="LogOut()">Log out</a></h1>';


    }
}

function LogOut()
{
    var request = $.ajax({
                                 url: "https://pizzacrash.servebeer.com/get/pizza",
                                 method: "GET",
                                 dataType: "json"
                             });


        request.done(function(data) {
            var MaxPizzas=JSON.parse(localStorage.getItem("PizzaArray")).Pizzas.length;
                var temp=[];
                for(let i=0;i<MaxPizzas;i++)
                    temp.push(0);   
                localStorage.setItem("PickedPizzas",JSON.stringify(temp));
            
            
        });


        request.fail(function(jqXHR, textStatus) {
            console.error("Error: " + textStatus);
        });
    });
    Cookies.remove('email', { path: '', domain: 'pizzacrash.servebeer.com' })
    location.reload();
}
