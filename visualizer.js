var canvas = document.getElementById("canvas");
var scounter = document.getElementById("step_counter");
var svalue = document.getElementById("sscroll");
var field = {}, unit = {}, antfield = {};
var moves;
canvas.height = 600;
canvas.width = 760;
textMargin = 5;
textHeight = 25;
field.x = 0;
field.y = textHeight;
field.width = canvas.width;
field.height = canvas.height - 2 * textHeight;
antfield.width = 31;
antfield.height = 31;
unit.width = field.width / antfield.width;
unit.height = field.height / antfield.height;

var cells = [];
var food = [];

var K = 20;
var foods = [];

var cur_food;

function clear_food() {
  for (i = 0; i < antfield.width; i++) {
    food[i] = [];  
    for (j = 0; j < antfield.height; j++) {
      food[i][j] = 0;
    }
  }
}

function update_food(move) {
  var j;
  for (j = 0; j < moves[move][2].length; j++)
    food[moves[move][2][j][0] - 1][moves[move][2][j][1] - 1] = moves[move][2][j][2];
}

teamNames = ["team1", "team2", "team3", "team4"];
colors = ['#FFA48E', '#9F70ED', '#609CFF', '#9CFF60'];
aligns = ["left", "right", "left", "right"];
textCords = [
{x: textMargin, y: textHeight- textMargin}, 
{x: canvas.width - textMargin, y: textHeight - textMargin}, 
{x: textMargin, y: canvas.height - textMargin},
{x: canvas.width - textMargin, y: canvas.height - textMargin}
];

ctx = canvas.getContext('2d');
ctx.lineWidth = Math.floor(textMargin / 2);
ctx.font = "bold 14pt Monospace";

var loadTimer = setInterval(show_loading, 250);
var loadcnt = 0;

function show_loading() {
  ctx.clearRect(0, 0, canvas.width, canvas.height);
  ctx.textAlign = "center";
  ctx.textBaseLine = "middle";
  txt = "Загрузка";
  for (i = 0; i < loadcnt; i++) {
      txt = txt + ".";
  }
  ctx.fillStyle = "black";
  ctx.fillText(txt, Math.floor(canvas.width / 2), Math.floor(canvas.height / 2));
  ctx.strokeStyle = "black";
  ctx.strokeRect(0, 0, canvas.width, canvas.height);
  loadcnt = (loadcnt + 1) % 5;
}

var timerID = null;
var speed = 40;
var cur = 0;

function changeSpeed() {
  speed = 20 + 5 * (60 - svalue.value);
  if (timerID != null) {
    clearInterval(timerID);
    timerID = setInterval(step, speed);
  } 
}

function get_food(cur) {
  if (cur_food + 1 == cur) {
    update_food(cur);
  }
  else {
    clear_food();
    console.log(cur);
    console.log(moves.length / K);
    var i = Math.floor(cur / K) * K;    
    console.log(i);
    cur_food = i;    
    for (xx = 0; xx < antfield.width; xx++) {
      for (yy = 0; yy < antfield.height; yy++) {
        food[xx][yy] = foods[i / K][xx][yy];
      }
    }   
    for (j = i + 1; j < cur; j++) {
      update_food(j);
    }    
  }   
  cur_food = cur;
}

function jump(dx) {
  if (typeof(moves) == "undefined")
    return;
  if (cur + dx - 1 < 0)
    cur = 0;
  else if (cur + dx - 1 >= moves.length)
    cur = moves.length - 1;
  else
    cur += dx - 1;
  step();
}

function step() {
  if (cur >= moves.length)
    return;
  if (typeof(moves) == "undefined")
    return;
  cells = [];
  for (i = 0; i < antfield.width; i++) {
    cells[i] = [];
    for (j = 0; j < antfield.height; j++) {
      cells[i][j] = [null, null, null, null];   
    }
  }

  ctx.clearRect(0, 0, canvas.width, canvas.height);
  //alert("DRAW");
  get_food(cur);
  draw_all(moves[cur]);
  //alert("DRAWED");
  scounter.innerHTML = "Ход " + (cur + 1) + "/" + moves.length + ".";
  cur = cur + 1;
  if (cur >= moves.length)
    stop();     
}




function play() {   
  if (timerID != null) {
    clearInterval(timerID);
    timerID = null;
  }
  timerID = setInterval(step, speed);   
}


function restart() {
  clear_food();    
  play();
  cur = 0;
  cur_food = -1;
}

function stop() {
  if (timerID != null)
    clearInterval(timerID); 
  timerID = null;
}

function add_ant(id, teamId, x, y) {
  //console.log("Team = " + teamId + " (" + x + ", " + y + ")");
  if (teamId >= 4)
    teamId -= 4;
  cells[x][y][teamId] = id;
}
  
function draw_ant_cell(data, x, y) {
  cnt = 0;  
  for (i = 0; i < 4; i++)
    if (cells[x][y][i] != null)
      cnt++;
  if (cnt == 0)
    return;        
  var points = []; 
  if (cnt == 1) {   
    points[0] = {x: 1 + unit.width * x, y: 1 + textHeight + unit.height * y, width: unit.width - 2, height: unit.height - 2};    
  }
  else if (cnt == 2) {    
    points[0] = {x: 1 + unit.width * x, y: 1 + textHeight + unit.height * y, width: Math.floor(unit.width / 2) - 1, height: unit.height - 2};
    points[1] = {x: 1 + unit.width * x + points[0].width, y: 1 + textHeight + unit.height * y, 
    width: Math.floor(unit.width / 2) - 1, height: unit.height - 2};
  }
  else {    
    points[0] = {x: 1 + unit.width * x, y: 1 + textHeight + unit.height * y, 
    width: Math.floor(unit.width / 2) - 1, height: Math.floor(unit.height / 2) - 1};
    points[1] = {x: 1 + unit.width * x + points[0].width, y: 1 + textHeight + unit.height * y, 
    width: points[0].width, height: points[0].height};
    points[2] = {x: 1 + unit.width * x, y: 1 + textHeight + unit.height * y + points[0].height, 
    width: points[0].width, height: points[0].height};
    points[3] = {x: 1 + unit.width * x + points[0].width, y: 1 + textHeight + unit.height * y + points[0].height, 
    width: points[0].width, height: points[0].height};
  }
  //alert(cells[x][y]);
  j = 0;
  for (i = 0; i < cells[x][y].length; i++) {
    if (cells[x][y][i] != null) {
      draw_ant(points[j], data[1][cells[x][y][i]][2], data[1][cells[x][y][i]][3], data[1][cells[x][y][i]][4]);
      j++;
    }
  }
}
 
function draw_ant(point, teamId, hasFood, isBitten) {     
  if (typeof(point) == "undefined")
    return;
  var dead = false;
  if (teamId >= 4) {
    dead = true;
    teamId -= 4;
  }
  ctx.fillStyle = colors[teamId];
  //ctx.clearRect(point.x, point.y, point.width, point.height); 
  ctx.lineWidth = 1;
  ctx.fillRect(point.x, point.y, point.width, point.height);
  ctx.strokeStyle = 'black';          
  ctx.strokeRect(point.x, point.y, point.width, point.height);         
  if (dead) {
    ctx.beginPath();
    ctx.lineCap = 'round';
    ctx.moveTo(point.x, point.y);
    ctx.lineTo(point.x + point.width, point.y + point.height);
    ctx.moveTo(point.x + point.width, point.y);
    ctx.lineTo(point.x, point.y + point.height);    
    ctx.lineWidth = 5;
    ctx.strokeStyle = 'red';
    ctx.stroke();
  }
  
  if (isBitten) {
    ctx.beginPath();
    ctx.lineCap = 'round';
    ctx.moveTo(point.x, point.y);
    ctx.lineTo(point.x + point.width, point.y + point.height);
    ctx.moveTo(point.x + point.width, point.y);
    ctx.lineTo(point.x, point.y + point.height);
    ctx.strokeStyle = 'black';        
    ctx.lineWidth = 2;
    ctx.stroke();
  }
  if (hasFood) {
    ctx.beginPath();
    ctx.arc(point.x + Math.floor(point.width / 2), point.y + Math.floor(point.height / 2) + 1, 
            Math.floor(4 * Math.min(point.width, point.height) / 10), 0, 2 * Math.PI, false);
    ctx.fillStyle = 'white';
    ctx.fill();
    ctx.lineWidth = 1;
    ctx.strokeStyle = 'black';
    ctx.stroke();
  }
}
  
function draw_food(x, y, intensity) {
  point = {};
  point.x = unit.width * x;
  point.y = unit.height * y + textHeight;
  intensityx = 255 - intensity;
  ctx.fillStyle = "rgb(" + intensityx + "," + intensityx + "," + intensityx + ")";
  ctx.beginPath();
  ctx.arc(point.x + Math.floor(unit.width / 2), point.y + Math.floor(unit.height / 2), Math.floor(1 * Math.min(unit.width, unit.height) / 2), 
  0, 2 * Math.PI, false);     
  //console.log();
  ctx.fill();
  ctx.lineWidth = 2;
  if (intensity > 0)
    ctx.stroke();
}


function draw_all(data) {

  ctx.fillStyle = colors[0];
  ctx.fillRect(textMargin, textMargin, Math.floor(canvas.width / 2) - 2 * textMargin, textHeight - 2 * textMargin);
  ctx.fillStyle = colors[1];
  ctx.fillRect(Math.floor(canvas.width / 2) +  textMargin, textMargin, 
             Math.floor(canvas.width / 2) - 2 * textMargin, textHeight - 2 * textMargin);
  ctx.fillStyle = colors[2];
  ctx.fillRect(textMargin, canvas.height - textHeight + textMargin, Math.floor(canvas.width / 2) - 2 * textMargin, textHeight - 2 * textMargin);
  ctx.fillStyle = colors[3];
  ctx.fillRect(Math.floor(canvas.width / 2) +  textMargin, canvas.height - textHeight + textMargin, 
             Math.floor(canvas.width / 2) - 2 * textMargin, textHeight - 2 * textMargin);
  ctx.textBaseLine = "bottom";
  for (i = 0; i < data[0].length; i++) {
    ctx.textAlign = aligns[i];  
    ctx.fillStyle = "black";
    ctx.fillText(teamNames[i] + ": " + data[0][i], textCords[i].x, textCords[i].y);
    //ctx.strokeText(data[0][i], textCords[i].x, textCords[i].y);
  }
  
  
  for (i = 0; i < antfield.width; i++) {
    for (j = 0; j < antfield.height; j++) {
      if (food[i][j])
        draw_food(i, j, food[i][j]);
    }
  }
  for (i = 0; i < data[1].length ; i++) {
    add_ant(i, data[1][i][2], data[1][i][0] - 1, data[1][i][1] - 1);
  }
  
  
  //alert("DRAW_CELLS");
 
  for (ii = 0; ii < antfield.width; ii++) {
    for (jj = 0; jj < antfield.height; jj++) {
      draw_ant_cell(data, ii, jj);
    }
  }

  ctx.strokeStyle = 'black';
  ctx.lineWidth = 2;  
  ctx.strokeRect(field.x, field.y, field.width, field.height);
  ctx.setLineDash([5]);
  ctx.strokeRect(0, 0, canvas.width, canvas.height);
  ctx.setLineDash([0]);    
}

function precalc_food() {
  clear_food();
  console.log(moves.length);
  for (i = 0; i < moves.length; i++) {
    update_food(i);
    if (i % K == 0) {
      foods[i / K] = [];
      for (xx = 0; xx < antfield.width; xx++) {
        foods[i / K][xx] = [];
        for (yy = 0; yy < antfield.height; yy++) {
          foods[i / K][xx][yy] = food[xx][yy];
        }
      }
      console.log(i);
    }    
  }
}

var id = window.location.search.substring(1);

if (typeof($) != "function") {
  alert("Ошибка соединения.");
  clearInterval(loadTimer);
  moves = [[[0,0,0,0],[],[]]];
  step();  
}
else {
  $.getJSON("./names.json", function (data) {
    teamNames = data;
  });
  $.getJSON("./res.json", function(data) {  
    moves = data;  
    precalc_food(); 
    clear_food();
    clearInterval(loadTimer);
    cur = 0;
    cur_food = -1;
    step();  
  }
  ).fail(function() {          
      alert("Ошибка при загрузке протокола игры.");
      moves = [[[0,0,0,0],[],[]]];
      clearInterval(loadTimer);
      step();    
  });

}
