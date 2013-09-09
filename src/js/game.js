function Squarez(board, rootElement, scoreElement)
{
	this.root = rootElement;
	this.board = board;
	this.selection = new Module.Selection();
	this.scoreElement = scoreElement
	this.score = 0;
	
	this.timer = new Module.Timer(10, 60, 180);
	var that = this;
	var timerEl = rootElement.getElementsByClassName("timeLeftInner")[0];
	this.timerFunc = setInterval(function()
	{
		var timeLeft = that.timer.percentageLeft()
		timerEl.style.width = ""+timeLeft+"%";
		if (timeLeft == 0)
			that.gameOver();
	}, 60);
	
	var fontSize = window.getComputedStyle(rootElement).fontSize.match(/[0-9]*/)[0];
	while (document.body.clientWidth <= rootElement.clientWidth)
	{
		fontSize -= 1;
		rootElement.style.fontSize = ""+fontSize+"px";
	}

	var size = board.size();
	for (var y = 0 ; y < size ; y++)
	{
		for (var x = 0 ; x < size ; x++)
		{
			this.addElement(x,y,board.get(x,y));
		}
	}
	if (this.root.ontouchstart === undefined)
		this.root.classList.add("no-touch");
	
	var pauseButtons = document.getElementsByClassName("jsPause");
	for (var i = 0 ; i < pauseButtons.length; i++)
	{
		if (this.root.ontouchstart === undefined)
			pauseButtons[i].onclick = function() {that.pause();};
		else
			pauseButtons[i].ontouchstart = function() {that.pause();};
	}
	
	var clearButtons = document.getElementsByClassName("jsClear");
	for (var i = 0 ; i < clearButtons.length; i++)
	{
		if (this.root.ontouchstart === undefined)
			clearButtons[i].onclick = function() {that.clearSelection();};
		else
			clearButtons[i].ontouchstart = function() {that.clearSelection();};
	}
	
	this.getHighScores(true);
}

Squarez.prototype =
{
	addElement: function(x,y,symbol)
	{
		var n = document.createElement("div");
		n.classList.add("cell");
		n.classList.add("x"+x);
		n.classList.add("y"+y);
		n.classList.add("symbol"+symbol);
		var squarez = this;
		if (n.ontouchstart !== undefined)
		{
			n.ontouchstart = function() {squarez.select(this);};
		}
		else
		{
			n.onclick = function() {squarez.select(this);}
		}
		this.root.appendChild(n);
		return n;
	},

	getPosition: function(el)
	{
		var rx = /x([0-9]+)/;
		var ry = /y([0-9]+)/;
		return {
			x: parseInt(el.className.match(rx)[1]),
			y: parseInt(el.className.match(ry)[1])
		};
	},

	getCell: function(x, y)
	{
		var cells = this.root.getElementsByClassName("x"+x+" y"+y);
		for (var i = 0 ; i < cells.length ; i++)
		{
			if (! cells[i].classList.contains("transient"))
				return cells[i];
		}
	},

	updateBoard: function(transition)
	{
		var old = this.root.getElementsByClassName("transient");
		while (old.length != 0)
		{
			old[0].parentNode.removeChild(old[0]);
		}

		if (transition.score == 0)
			return;
		
		this.timer.refill(2*transition.score);
		
		this.score += transition.score;
		this.scoreElement.innerHTML = this.score;

		var size = this.board.size();
		var transitionSize = transition.size();
		
		var moveTo = function(fromx, fromy, x, y, cell)
		{
			cell.classList.remove("x"+fromx);
			cell.classList.remove("y"+fromy);
			cell.classList.add("x"+x);
			cell.classList.add("y"+y);
			if ((fromx - x) * (fromx - x) + (fromy - y) * (fromy - y) == 4)
			{
				cell.classList.add("doubleMove");
			}
			else
			{
				cell.classList.remove("doubleMove");
			}
		};
		
		var removed = new Array();
		
		for (var i = 0 ; i < transitionSize ; i++)
		{
			var cell = transition.get(i);
			if (cell.removed)
			{
				var e = this.getCell(cell.fromx, cell.fromy);
				e.classList.add("removed");
				e.classList.add("transient");//FIXME: Firefox does not support add with multiple arguments
				removed.push({x:cell.fromx, y:cell.fromy, symbol:this.board.get(cell.fromx, cell.fromy)});
			}
			else
			{
				if (cell.isNew())
				{
					setTimeout(moveTo, 100, cell.fromx, cell.fromy, cell.tox, cell.toy,
						this.addElement(cell.fromx, cell.fromy, cell.symbol));
				}
				else
				{
					setTimeout(moveTo, 0, cell.fromx, cell.fromy, cell.tox, cell.toy,
						this.getCell(cell.fromx, cell.fromy));
				}
			}
		}
		if (removed.length == 4)
		{
			var center = {x:0, y:0};
			for (var i = 0 ; i < 4 ; i++)
			{
				center.x += removed[i].x;
				center.y += removed[i].y;
			}
			center.x /=4;
			center.y /=4;
			var squareSize = (removed[0].x - removed[1].x)*(removed[0].x - removed[1].x) + (removed[0].y - removed[1].y)*(removed[0].y - removed[1].y);
			var side = 1;
			var otherSize = (removed[0].x - removed[2].x)*(removed[0].x - removed[2].x) + (removed[0].y - removed[2].y)*(removed[0].y - removed[2].y);
			if (otherSize < squareSize)
			{
				squareSize = otherSize;
				side = 2;
			}
			squareSize = Math.sqrt(squareSize);
			var n = document.createElement("div");
			n.style.top = ""+(0.5+center.y)+"em";
			n.style.left = ""+(0.5+center.x)+"em";
			n.classList.add("transient");
			n.classList.add("transition-square-container");
			n.style.position = "absolute";
			var n1 = document.createElement("div");
			n.appendChild(n1);
			n1.style.marginLeft = "-50%";
			n1.style.marginTop = "-50%";
			n1.style.width = ""+squareSize + "em";
			n1.style.height = ""+squareSize + "em";
			n1.classList.add("transition-square");
			n1.classList.add("symbol"+removed[0].symbol);
			var rotation = Math.atan2(removed[0].y - removed[side].y, removed[0].x - removed[side].x);
			if (n.style.transform !== undefined)
			{
				n.style.transform = "rotate("+rotation+"rad)";
				n.style.transformOrigin = "0 0"
			}
			if (n.style.webkitTransform !== undefined)
			{
				n.style.webkitTransform = "rotate("+rotation+"rad)";
				n.style.webkitTransformOrigin = "0 0"
			}
			setTimeout(function() {n.classList.add("transition-removed");}, 100)
			this.root.appendChild(n);
		}
		this.board.applyTransition(transition);
	},

	select: function(el)
	{
		var pos = this.getPosition(el);
		var selected = this.selection.addPoint(pos.x, pos.y);
		if (selected)
		{
			el.classList.add("selected");
			var transition = this.board.selectSquare(this.selection);
			if (transition.score > 0)
			{
				this.clearSelection();
				this.updateBoard(transition);
			}
		}
		else
		{
			el.classList.remove("selected");
		}
	},
	
	gameOver: function()
	{
		clearInterval(this.timerFunc);
		this.root.getElementsByClassName("gameOver")[0].style.display = "";
		var scores = this.getHighScores();
		var that = this;
		if (this.score > 0 && (scores.length < 10 || this.score >= scores[scores.length - 1].score))
		{
			var form = document.getElementById("nameForm");
			form.style.display = "";
			if (typeof(Storage) !== "undefined" && sessionStorage["lastname"])
				document.getElementById("nameInput").value = sessionStorage["lastname"];
			form.onsubmit = function()
			{
				var name = document.getElementById("nameInput").value;
				if (! name)
					return false;
				form.style.display = "none";
				sessionStorage["lastname"] = name;
				that.saveScore(name);
				that.getHighScores(true);
				return false;
			}
		}
	},
	
	pause: function()
	{
		if (this.timer.paused())
		{
			this.timer.unPause();
			this.root.getElementsByClassName("pause")[0].style.display = "none";
		}
		else
		{
			this.timer.pause();
			this.root.getElementsByClassName("pause")[0].style.display = "";
		}
	},
	
	clearSelection: function()
	{
		this.selection.delete();
		this.selection = new Module.Selection();
		var selected = this.root.getElementsByClassName("selected");
		while (selected.length != 0)
		{
			selected[0].classList.remove("selected");
		}
	},
	
	getHighScores: function(draw)
	{
		var res = new Array();
		if (typeof(Storage) === "undefined")
			return res;
		for (var i = 0 ; localStorage["score_val_"+i]; i++)
		{
			res.push({
				score: parseInt(localStorage["score_val_"+i]),
				date: new Date(parseInt(localStorage["score_date_"+i])),
				name: localStorage["score_name_"+i]
			});
		}
		if (draw)
		{
			var scoreList = document.getElementById("scoreList");
			scoreList.innerHTML = "";
			for (var i = 0 ; i < res.length ; i++)
			{
				document.getElementById("highScores").style.display="";
				var n = document.createElement("tr");
				n.innerHTML="<td>"+res[i].name+ "</td><td>"+ res[i].date.toLocaleDateString() + "</td><td>" + res[i].score + "</td>";
				scoreList.appendChild(n);
			}
		}
		return res;
	},
	
	saveScore: function(name)
	{
		if (typeof(Storage) === "undefined" || this.score == 0)
			return false;
		var scores = this.getHighScores();
		for (var i = 0 ; i < scores.length ; i++)
		{
			if (this.score >= scores[i].score)
			{
				// Shift existing scores
				for (var j = Math.min(scores.length, 9); j > i ; j--)
				{
					localStorage["score_val_"+j] = localStorage["score_val_"+(j-1)];
					localStorage["score_date_"+j] = localStorage["score_date_"+(j-1)];
					localStorage["score_name_"+j] = localStorage["score_name_"+(j-1)]
				}
				break;
			}
		}
		if (i < 9)
		{
			localStorage["score_val_"+i] = this.score;
			localStorage["score_date_"+i] = Date.now();
			localStorage["score_name_"+i] = name;
			return true;
		}
		return false;
	}
}