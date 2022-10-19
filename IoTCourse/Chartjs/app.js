$(document).ready(function(){
  $.ajax({
    url: "https://faridfarahmand.000webhostapp.com/Chartjs/data.php",
    method: "GET",
    success: function(data) {
      console.log(data);
      var Node_ID = [];
      var Node_Temp = [];

      for(var i in data) {
        Node_ID.push("N:" + data[i].id);
        Node_Temp.push(data[i].temperature);
      }

      var chartdata = {
        labels: Node_ID,
        datasets : [
          {
            label: 'Temperature',
            backgroundColor: 'rgba(200, 200, 200, 0.75)',
            borderColor: 'rgba(200, 200, 200, 0.75)',
            hoverBackgroundColor: 'rgba(200, 200, 200, 1)',
            hoverBorderColor: 'rgba(200, 200, 200, 1)',
            data: Node_Temp
          }
        ]
      };

      var ctx = $("#mycanvas");

      var barGraph = new Chart(ctx, {
        type: 'bar',
        data: chartdata
      });
    },
    error: function(data) {
      console.log(data);
    }
  });
});