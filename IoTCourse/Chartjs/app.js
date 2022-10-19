$(document).ready(function(){
  $.ajax({
    url: "http://localhost/chartjs/data.php",
    method: "GET",
    success: function(data) {
      console.log(data);
      var nodeid = [];
      var ndetemp = [];

      for(var i in data) {
        nodeid.push("Player " + data[i].id);
        nodetemp.push(data[i].temperature);
      }

      var chartdata = {
        labels: player,
        datasets : [
          {
            label: 'Player Score',
            backgroundColor: 'rgba(200, 200, 200, 0.75)',
            borderColor: 'rgba(200, 200, 200, 0.75)',
            hoverBackgroundColor: 'rgba(200, 200, 200, 1)',
            hoverBorderColor: 'rgba(200, 200, 200, 1)',
            data: score
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