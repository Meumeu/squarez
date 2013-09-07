var Squarez = {
	selection : new Module.Selection()
	};

Squarez.addElement = function(x,y,symbol, board, root)
{
	var n = document.createElement("div");
	n.classList.add("cell");
	n.classList.add("x"+x);
	n.classList.add("y"+y);
	n.classList.add("symbol"+symbol);
	n.onclick = function() {
		Squarez.select(board, this, root);
	}
	root.appendChild(n);
	return n;
}

Squarez.drawBoard = function(board, el)
{
	var size = board.size();
	for (var y = 0 ; y < size ; y++)
	{
		for (var x = 0 ; x < size ; x++)
		{
			Squarez.addElement(x,y,board.get(x,y), board, el);
		}
	}
	Squarez.setListeners(board, el);
};

Squarez.getPosition = function(el)
{
	var rx = /x([0-9]+)/;
	var ry = /y([0-9]+)/;
	return {
		x: el.className.match(rx)[1]*1,
		y: el.className.match(ry)[1]*1
	};
}

Squarez.getCell = function(x, y, el)
{
	var cells = el.getElementsByClassName("x"+x+" y"+y);
	for (var i = 0 ; i < cells.length ; i++)
	{
		if (! cells[i].classList.contains("removed"))
			return cells[i];
	}
}

Squarez.updateBoard = function(board, transition, el)
{
	board.applyTransition(transition);
	var old = el.getElementsByClassName("removed");
	for (var i = 0 ; i < old.length ; i++)
	{
		old[i].remove();
	}

	if (transition.score == 0)
		return;

	var size = board.size();
	for (var x = 0 ; x < size ; x++)
	{
		var cells = new Array();
		for (var y = 0 ; y < size ; y++)
		{
			cells[y] = Squarez.getCell(x,y,el);
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
		var cellEl = Squarez.addElement(newCell.x, newCell.y - move[newCell.x], newCell.symbol, board, el);
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
}

Squarez.select = function(board, el, root)
{
	var pos = Squarez.getPosition(el);
	var selected = this.selection.addPoint(pos.x, pos.y);
	if (selected)
	{
		el.classList.add("selected");
		var transition = board.selectSquare(this.selection);
		if (transition.score > 0)
		{
			this.selection.delete();
			this.selection = new Module.Selection();
			Squarez.updateBoard(board, transition, root);
		}
	}
	else
	{
		el.classList.remove("selected");
	}
}

Squarez.setListeners = function(board, el)
{
	var cells = el.getElementsByClassName("cell");
	for (var i = 0; i < cells.length; i++)
	{
	}
}
