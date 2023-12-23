var CenterPizza=0;
var LeftPizza=0;
var RightPizza=0;
var MaxPizzas=0;

getPizzaList()

function getPizzaList()
{
    $(document).ready(function(){

        var request = $.ajax({
                                 url: "https://pizzacrash.servebeer.com/get/pizza",
                                 method: "GET",
                                 dataType: "json"
                             });


        request.done(function(data) {
            console.log("Data:", data);
            // Здесь вы можете обработать полученные данные
        });


        request.fail(function(jqXHR, textStatus) {
            console.error("Error: " + textStatus);
        });
    });
}

function ChangePizza(n)
{

}

function mod(n, m)
{
    return ((n % m) + m) % m;
}
function scrollPizzas(n)
{
    CenterPizza=mod(CenterPizza+n,MaxPizzas);
    LeftPizza=mod(CenterPizza-1,MaxPizzas);
    RightPizza=mod(CenterPizza+1,MaxPizzas);
    document.getElementById("LeftPic").src=pizzaResp[LeftPizza]["Image link"];
    document.getElementById("CenterPic").src=pizzaResp[CenterPizza]["Image link"];
    document.getElementById("RightPic").src=pizzaResp[RightPizza]["Image link"];
}
