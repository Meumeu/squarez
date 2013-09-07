function Squarez(board, rootElement)
{
	this.root = rootElement;
	this.board = board;
	this.selection = new Module.Selection();

	var size = board.size();
	for (var y = 0 ; y < size ; y++)
	{
		for (var x = 0 ; x < size ; x++)
		{
			this.addElement(x,y,board.get(x,y));
		}
	}
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
		n.onclick = function() {
			squarez.select(this);
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
			if (! cells[i].classList.contains("removed"))
				return cells[i];
		}
	},

	updateBoard: function(transition)
	{
		this.board.applyTransition(transition);

		var old = this.root.getElementsByClassName("removed");
		for (var i = 0 ; i < old.length ; i++)
		{
			old[i].remove();
		}

		if (transition.score == 0)
			return;

		var size = this.board.size();
		for (var x = 0 ; x < size ; x++)
		{
			var cells = new Array();
			for (var y = 0 ; y < size ; y++)
			{
				cells[y] = this.getCell(x,y);
			}
			for (var y = 0 ; y < size ; y++)
			{
				if (transition.getRemoved(x,y))
				{
					cells[y].classList.add("removed");
				}
				else if (transition.getMove(x,y)> 0)
				{
					cells[y].classList.remove("y"+y);
					cells[y].classList.remove("doubleMove");
					if (transition.getMove(x,y) == 2)
						cells[y].classList.add("doubleMove");
					cells[y].classList.add("y"+(y+transition.getMove(x,y)));
				}
			}
		}

		var move = {};
		for (i = 0 ; i < 4 ; i++)
		{
			var newCell = transition.getNewCell(i);
			if (! move[newCell.x])
			{
				move[newCell.x] = 1;
			}
			else
			{
				move[newCell.x] += 1;
			}
		}
		for (i = 0 ; i < 4 ; i++)
		{
			var newCell = transition.getNewCell(i);
			Module.print("new cell " +newCell.symbol+ " x:"+newCell.x + " y:" + newCell.y); 
			var classes = "";
			var cellEl = this.addElement(newCell.x, newCell.y - move[newCell.x], newCell.symbol);
			if (move[newCell.x] == 2)
			{
				cellEl.classList.add("doubleMove");
			}

			setTimeout( function(element, from, to)
			{
				element.classList.remove("y"+from);
				element.classList.add("y"+to);
			}, 1, cellEl, newCell.y - move[newCell.x], newCell.y);
		}
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
