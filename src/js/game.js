function Squarez(board, rootElement, scoreElement)
{
	this.root = rootElement;
	this.board = board;
	this.selection = new Module.Selection();
	this.scoreElement = scoreElement
	this.score = 0;
	
	var fontSize = window.getComputedStyle(rootElement).fontSize.match(/[0-9]*/)[0];
	while (document.body.clientWidth < rootElement.clientWidth)
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
			x: el.className.match(rx)[1]*1,
			y: el.className.match(ry)[1]*1
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
		for (var i = 0 ; i < old.length ; i++)
		{
			old[i].parentNode.removeChild(old[i]);
		}

		if (transition.score == 0)
			return;
		
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
				this.selection.delete();
				this.selection = new Module.Selection();
				this.updateBoard(transition);
			}
		}
		else
		{
			el.classList.remove("selected");
		}
	}
}
