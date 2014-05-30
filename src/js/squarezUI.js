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

	var size = gameRules.getBoard().size();
	for (var y = 0 ; y < size ; y++)
	{
		for (var x = 0 ; x < size ; x++)
		{
			this.addElement(x,y,gameRules.getBoard().get(x,y));
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

	var tutorialOverlay = document.getElementsByClassName("tutorialOverlay");
	for (var i = 0 ; i < tutorialOverlay.length; i++)
	{
		if (this.root.ontouchstart === undefined)
			tutorialOverlay[i].onclick = function() {that.rules.next();};
		else
			tutorialOverlay[i].ontouchstart= function() {that.rules.next();};
	}

	var gameOverFunc = null

}

function drawScoreList(scoreList)
{
	var scoreListEl = document.getElementById("scoreList");
	scoreListEl.innerHTML = "";
	for (var i = 0 ; i < scoreList.size() ; i++)
	{
		document.getElementById("highScores").style.display="";
		var n = document.createElement("tr");
		var d = new Date(scoreList.get(i).date * 1000);
		n.innerHTML='<td class="name">'+scoreList.get(i).name+ '</td><td class="date">'+ d.toLocaleDateString() + '</td><td class="score">' + scoreList.get(i).score + "</td>";
		scoreListEl.appendChild(n);
	}
};

SquarezUI.prototype =
{
	addElement: function(x,y,symbol)
	{
		var n = document.createElement("div");
		n.classList.add("cell");
		n.dataset.x = x;
		n.dataset.y = y;
		n.style.left=x+"em";
		n.style.top=y+"em";
		n.classList.add("symbol"+symbol);
		var that = this;
		if (n.ontouchstart !== undefined)
		{
			n.ontouchstart = function(event) {event.preventDefault(); that.select(this);};
		}
		else
		{
			n.onclick = function() {that.select(n);}
		}
		this.root.appendChild(n);
		return n;
	},

	getPosition: function(el)
	{
		return {
			x: parseInt(el.dataset.x),
			y: parseInt(el.dataset.y)
		};
	},

	getCell: function(x, y)
	{
		return this.root.querySelector('[data-x="'+x+'"][data-y="'+y+'"]');
	},

	onSelectionAccepted: function(selection)
	{
		this.clearSelection();
		this.clearAnimations();
		var n = this.drawSelection(selection);
		n.classList.add("userSelection");
	},
	
	onSelectionRejected: function(selection)
	{
		this.clearSelection();
	},

	drawSelection: function(selection)
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
		var symbol = this.rules.getBoard().get(removed[0].x, removed[0].y);

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
		var n = this.drawSelection(selection);
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

	onSelectionApplied: function(selection)
	{
		this.animateSelection(selection);
	},

	onTransition: function(transition)
	{

		var transitionSize = transition.size();
		if (transitionSize == 0)
			return;

		this.clearSelection();
		this.clearAnimations();

		var packed = this.root.getElementsByClassName("packed-move");
		while(packed.length != 0)
		{
			// Reparent the inner elements
			for (var i = 0 ; i < packed[0].children.length ; i++)
				this.root.appendChild(packed[0].children[i]);
			packed[0].parentNode.removeChild(packed[0]);
		}

		var pack = {
			1: document.createElement("div"),
			2: document.createElement("div")};

		pack[1].classList.add("move-1");
		pack[2].classList.add("move-2");

		this.root.appendChild(pack[1]);
		this.root.appendChild(pack[2]);
		
		var moveTo = function(fromx, fromy, x, y, cell)
		{
			cell.dataset.x = x;
			cell.dataset.y = y;
			cell.style.left=x+"em";
			cell.style.top=y+"em";
			if (fromx == x && fromy == y - 1)
				pack[1].appendChild(cell);
			else if (fromx == x && fromy == y - 2)
				pack[2].appendChild(cell);
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
				e.classList.add("transient");
				e.dataset.x = "";
				e.dataset.y = "";
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
	},

	getGlobalTimeLeft: function(offset) { return this.rules.getPercentageLeft(offset); },
	
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
		}
		else
		{
			el.classList.remove("selected");
		}
		this.rules.onSelect(this.selection);
	},

	onGameOverChanged: function(gameOver)
	{
		if (gameOver)
		{
			this.root.getElementsByClassName("gameOver")[0].style.display = "";
		}
	},

	onScoreChanged: function(score)
	{
		this.scoreElement.innerHTML = score;
	},

	pause: function()
	{
		if (this.rules.pause)
		{
			this.rules.pause = false;
			this.root.getElementsByClassName("pause")[0].style.display = "none";
		}
		else
		{
			this.rules.pause = true;
			this.root.getElementsByClassName("pause")[0].style.display = "";
		}
	},

	onNameRequired: function(lastName)
	{
		var form = document.getElementById("nameForm");
		form.style.display = "";
		document.getElementById("nameInput").value = lastName;
		var that = this;
		form.onsubmit = function()
		{
			var name = document.getElementById("nameInput").value;
			if (! name)
				return false;
			form.style.display = "none";
			that.rules.setPlayerName(name);
			return false;
		}
	},

	onMessage: function(message)
	{
		var container = document.getElementById("messageContainer");
		container.innerHTML = message.replace(/\n/g, "<br>");
		if (message.length == 0)
			container.style.display = "none";
		else
			container.style.display = "";
	},

	onTimerUpdated: function(percentageLeft, msLeft)
	{
		var animDuration = 300;//ms
		this.timerEl.style.transition = "right "+ animDuration + "ms ease-out, "+ animDuration + "ms ease-out";
		this.timerEl.style.webkitTransition = "right "+ animDuration + "ms ease-out, "+ animDuration + "ms ease-out";;
		this.timerEl.style.right = ""+((1-percentageLeft)*100)+"%";
		this.timerEl.style.top = this.timerEl.style.right;
		var that = this;
		setTimeout(function()
		{
			that.timerEl.style.transition = "right " + msLeft + "ms linear, top " + msLeft + "ms linear";
			that.timerEl.style.webkitTransition = "right " + msLeft + "ms linear, top " + msLeft + "ms linear";
			that.timerEl.style.right = "100%";
			that.timerEl.style.top = "100%";
		}, animDuration
		)
		if (this.gameOverFunc !== null)
		{
			window.clearTimeout(this.gameOverFunc);
		}
		this.gameOverFunc = window.setTimeout(function() { that.rules.getGameOver();}, msLeft + 1);
	},

	onScoreListChanged: drawScoreList
}
