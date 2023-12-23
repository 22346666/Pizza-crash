var TotalPrice=0;
var MaxPizzas=5;

function GetOrder()
{
  $(document).ready(function(){

        var request = $.ajax({
                                 url: "https://pizzacrash.servebeer.com/get/pizza",
                                 method: "GET",
                                 dataType: "json"
                             });


        request.done(function(data) {
            localStorage.setItem("PizzaArray",JSON.stringify(data));
            TotalPrice=0;
            MaxPizzas=JSON.parse(localStorage.getItem("PizzaArray")).Pizzas.length;
            if(localStorage.getItem("PickedPizzas")==null)
            {
                var temp=[];
                for(let i=0;i<MaxPizzas;i++)
                    temp.push(0);   
                localStorage.setItem("PickedPizzas",JSON.stringify(temp));
            }
            var PickedPizzas=localStorage.getItem("PickedPizzas");
            var PList=JSON.parse(localStorage.getItem("PizzaArray"));
            for(var i=0;i<MaxPizzas;i++)
            {
              TotalPrice+=PList.Pizzas[i]["Cost"]*PickedPizzas[i];
            }
            document.getElementById("TotalPrice").innerHTML="TOTAL: "+TotalPrice+"₴";
            document.getElementById("OrderList").innerHTML="";
            for(var i=0;i<MaxPizzas;i++)
            {
                if(PickedPizzas[i]>0)
                {
                     document.getElementById("OrderList").innerHTML+="<p><img src='"+PList.Pizzas[i]["Image link"]+"' style='max-width:100;'>"+
                       PList.Pizzas[i]["Name"]+
                       " x"+PickedPizzas[i]+": "+
                       PList.Pizzas[i]["Cost"]*PickedPizzas[i]+"₴</p>"
                }
            }
        });


        request.fail(function(jqXHR, textStatus) {
            console.error("Error: " + textStatus);
        });
    });
}

function SendOrder()
{
  var currentdate     = new Date(); 
  var OrderObj=new Object();
  OrderObj["Email"]=Cookies.get("email");
  OrderObj["Address"]=document.getElementById("DeliveryAddress").value;
  OrderObj["Summ"]=TotalPrice;
  OrderObj["Date"]=currentdate.getFullYear() + "-" +
                       ('0' + (currentdate.getMonth()+1)).slice(-2)  + "-"+
                       ('0' + currentdate.getDate()).slice(-2) + " "  + 
                       ('0' + currentdate.getHours()).slice(-2) + ":"   +
                       ('0' + currentdate.getMinutes()).slice(-2) + ":" + 
                       ('0' + currentdate.getSeconds()).slice(-2);

  $.post("https://pizzacrash.servebeer.com/post/order",
      JSON.stringify(OrderObj)
  )
}
