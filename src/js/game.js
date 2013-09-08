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
		
		for (var i = 0 ; i < transitionSize ; i++)
		{
			var cell = transition.get(i);
			if (cell.removed)
			{
				this.getCell(cell.fromx, cell.fromy).classList.add("removed");
			}
		}
		for (var i = 0 ; i < transitionSize ; i++)
		{
			var cell = transition.get(i);
			if (!cell.removed)
			{
				if (cell.isNew())
				{
					setTimeout(moveTo, 0, cell.fromx, cell.fromy, cell.tox, cell.toy,
						this.addElement(cell.fromx, cell.fromy, cell.symbol));
				}
				else
				{
					setTimeout(moveTo, 0, cell.fromx, cell.fromy, cell.tox, cell.toy,
						this.getCell(cell.fromx, cell.fromy));
				}
			}
		}
		this.board.print();
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
