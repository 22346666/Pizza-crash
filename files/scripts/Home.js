let CenterPizza=0;
let LeftPizza=0;
let RightPizza=0;
let MaxPizzas=5;

function getPizzaList()
{
    $(document).ready(function(){

        var request = $.ajax({
                                 url: "https://pizzacrash.servebeer.com/get/pizza",
                                 method: "GET",
                                 dataType: "json"
                             });


        request.done(function(data) {
            //console.log("Data:", data);
            localStorage.setItem("PizzaArray",JSON.stringify(data));
            CenterPizza=0;
            scrollPizzas(0);

            MaxPizzas=JSON.parse(localStorage.getItem("PizzaArray")).Pizzas.length;
            if(localStorage.getItem("PickedPizzas")==null)
            {
                var temp=[];
                for(let i=0;i<MaxPizzas;i++)
                    temp.push(0);   
                localStorage.setItem("PickedPizzas",JSON.stringify(temp));
            }
            // Здесь вы можете обработать полученные данные
        });


        request.fail(function(jqXHR, textStatus) {
            console.error("Error: " + textStatus);
        });
    });
}

function ChangePizza(n)
{
    var PP=JSON.parse(localStorage.getItem("PickedPizzas"));
    PP[CenterPizza]=Math.max(PP[CenterPizza]+1,0);
    localStorage.setItem("PickedPizzas",JSON.stringify(PP));

    DisplayPizzas();
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
    DisplayPizzas();
}

function DisplayPizzas()
{
    var PList=JSON.parse(localStorage.getItem("PizzaArray"));
    document.getElementById("LeftPic").src=PList.Pizzas[LeftPizza]["Image link"];
    document.getElementById("CenterPic").src=PList.Pizzas[CenterPizza]["Image link"];
    document.getElementById("RightPic").src=PList.Pizzas[RightPizza]["Image link"];

    document.getElementById("LeftName").innerHTML=PList.Pizzas[LeftPizza]["Name"];
    document.getElementById("CenterName").innerHTML=PList.Pizzas[CenterPizza]["Name"];
    document.getElementById("RightName").innerHTML=PList.Pizzas[RightPizza]["Name"];

    document.getElementById("LeftPrice").innerHTML=PList.Pizzas[LeftPizza]["Cost"];
    document.getElementById("CenterPrice").innerHTML=PList.Pizzas[CenterPizza]["Cost"];
    document.getElementById("RightPrice").innerHTML=PList.Pizzas[RightPizza]["Cost"];

    document.getElementById("PizzaCounter").innerHTML=JSON.parse(localStorage.getItem("PickedPizzas"))[CenterPizza];

    let TotalCounter=0;
    var PP=JSON.parse(localStorage.getItem("PickedPizzas"));
    for(var i=0;i<MaxPizzas;i++)
        TotalCounter+=PP[i];
     document.getElementById("TotalCounter").innerHTML="x"+(TotalCounter);
}
