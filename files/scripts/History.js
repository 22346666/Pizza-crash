var TotalPrice=0;
var UEmail;
var UHistory;

function GetHistory()
{
  $(document).ready(function(){
        UEmail=Cookies.get("email");
        var request = $.ajax({
                                 url: "https://pizzacrash.servebeer.com/get/orders/"+UEmail,
                                 method: "GET",
                                 dataType: "json"
                             });


        request.done(function(data) {
          UHistory=data;
          if(UHistory.Orders.length!=0)
          {
            document.getElementById("OrderHistory").innerHTML='';
            let i=1;
            let TableContents="<table><tr><th>№</th><th>Date</th><th>Address</th><th>Sum</th></tr>";
            for(i=0;i<UHistory["Orders"].length;i++)
            {
              TableContents+="<tr>"+
                "<td>"+(i+1)+"</td>"+
                "<td>"+UHistory["Orders"][i]["Date"]+"</td>"+
                "<td>"+UHistory["Orders"][i]["Address"]+"</td>"+
                "<td>"+UHistory["Orders"][i]["Summ"]+"₴</td>"+
                "</tr>";
            }
            TableContents+="</table>"
            console.log(TableContents);
            document.getElementById("OrderHistory").innerHTML+=TableContents;
          }
          else
          {
            document.getElementById("OrderHistory").innerHTML="<p>You didn't order anything.</p>";
          }
        });


        request.fail(function(jqXHR, textStatus) {
            console.error("Error: " + textStatus);
        });
    });
}
