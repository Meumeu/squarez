function SquarezUI(rootElement, scoreElement, timerCheat, gameRules)
{
	this.root = rootElement;
	this.selection = new Module.Selection();
	this.scoreElement = scoreElement

	this.rules = gameRules

	this.timerEl = document.getElementById("timerInner");
	// Amount of time in seconds that is hidden by the timer
	this.timerCheat = timerCheat;

	rootElement.classList.add("resizing");
	rootElement.style.fontSize = (Math.min(window.innerHeight, window.innerWidth)/8) + "px";
	window.onresize = function()
	{
		rootElement.classList.add("resizing");
		rootElement.style.fontSize = (Math.min(window.innerHeight, window.innerWidth)/8) + "px";
	}

	var size = gameRules.board.size();
	for (var y = 0 ; y < size ; y++)
	{
		for (var x = 0 ; x < size ; x++)
		{
			this.addElement(x,y,gameRules.board.get(x,y));
		}
	}

	if (this.root.ontouchstart === undefined)
		this.root.classList.add("no-touch");

	var that = this;

	var clearButtons = document.getElementsByClassName("jsClear");
	for (var i = 0 ; i < clearButtons.length; i++)
	{
		if (this.root.ontouchstart === undefined)
			clearButtons[i].onclick = function() {that.clearSelection();};
		else
			clearButtons[i].ontouchstart = function() {that.clearSelection();};
	}

	var pauseButtons = document.getElementsByClassName("jsPause");
	for (var i = 0 ; i < pauseButtons.length; i++)
	{
		if (this.root.ontouchstart === undefined)
			pauseButtons[i].onclick = function() {that.pause();};
		else
			pauseButtons[i].ontouchstart = function() {that.pause();};
	}

	this.updateTimer();

}
SquarezUI.prototype =
{
	addElement: function(x,y,symbol)
	{
		var n = document.createElement("div");
		var n1 = document.createElement("div");
		var n2 = document.createElement("div");
		n.classList.add("cell");
		n.classList.add("x"+x);
		n.classList.add("y"+y);
		n.classList.add("symbol"+symbol);
		n1.classList.add("cellShape");
		n2.classList.add("inner");
		n.appendChild(n1);
		n1.appendChild(n2);
		var that = this;
		if (n.ontouchstart !== undefined)
		{
			n.ontouchstart = function(event) {event.preventDefault(); that.select(this);};
		}
		else
		{
			n1.onclick = function() {that.select(n);}
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

	onSelectionAccepted: function(selection)
	{
		var removed = new Array();
		var center = {x:0, y:0};
		for (var i = 0 ; i < 4 ; i++)
		{
			removed.push({x: selection.getX(i), y: selection.getY(i)});
			center.x += removed[i].x;
			center.y += removed[i].y;
		}
		center.x /=4;
		center.y /=4;
		var symbol = this.rules.board.get(removed[0].x, removed[0].y);

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
		n1.classList.add("symbol"+symbol);
		var rotation = Math.atan2(removed[side].x - removed[0].x, removed[0].y - removed[side].y);
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
		this.root.appendChild(n);
		return n;
	},

	animateSelection: function(selection)
	{
		var n = this.onSelectionAccepted(selection);
		setTimeout(function() {n.classList.add("transition-removed");}, 100)
		return n;
	},

	clearAnimations: function()
	{
		var old = this.root.getElementsByClassName("transient");
		while (old.length != 0)
		{
			old[0].parentNode.removeChild(old[0]);
		}
	},

	onTransition: function(transition)
	{

		var transitionSize = transition.size();
		if (transitionSize == 0)
			return;

		this.clearSelection();
		
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
		var moves = new Array();
		
		this.root.classList.remove("resizing");
		
		for (var i = 0 ; i < transitionSize ; i++)
		{
			var cell = transition.get(i);
			if (cell.removed)
			{
				var e = this.getCell(cell.fromx, cell.fromy);
				e.classList.add("removed");
				e.classList.add("transient");//FIXME: Firefox does not support add with multiple arguments
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
					moves.push([cell.fromx, cell.fromy, cell.tox, cell.toy, this.getCell(cell.fromx, cell.fromy)])
				}
			}
		}
		for (var i = 0 ; i < moves.length; i++)
		{
			moveTo.apply(null, moves[i]);
		}
		if (transition.selection)
		{
			this.animateSelection(transition.selection);
		}
	},
	
	updateTimer: function()
	{
		if (!this.timerFunc)
		{
			var that = this;
			this.timerEl.style.transition = "right 0.5s linear, top 0.5s linear";
			this.timerEl.style.webkitTransition = "right 0.5s linear, top 0.5s linear";
			this.timerFunc = setInterval(function() {that.updateTimer();}, 500);
		}
		
		var timeLeft = this.rules.timer.percentageLeft(0.5 - this.timerCheat);
		this.timerEl.style.right = ""+((1-timeLeft)*100)+"%";
		this.timerEl.style.top = this.timerEl.style.right;

		//To be defined in rules methods
		this.onTimerEvent();
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

	select: function(el)
	{
		var pos = this.getPosition(el);
		var selected = this.selection.addPoint(pos.x, pos.y);
		if (selected)
		{
			el.classList.add("selected");
			this.rules.onSelect(this.selection);
		}
		else
		{
			el.classList.remove("selected");
		}
	},

	onTimerEvent: function()
	{
		if (this.rules.gameOver())
		{
			clearInterval(this.timerFunc);
			this.timerFunc = null;
			this.gameOver();
		}
	},
	
	onScoreChanged: function(score)
	{
		this.scoreElement.innerHTML = score;
	},

	pause: function()
	{
		if (this.rules.timer.paused())
		{
			this.rules.unPause();
			this.root.getElementsByClassName("pause")[0].style.display = "none";
			this.updateTimer();
		}
		else
		{
			this.rules.pause();
			clearInterval(this.timerFunc);
			this.timerFunc = null;
			this.root.getElementsByClassName("pause")[0].style.display = "";
		}
	}
}
