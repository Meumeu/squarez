function MultiplayerRules(rootElement, scoreElement, server)
{
	this.server = server;
	//Get the board from network
	var boardRequest = new XMLHttpRequest();
	boardRequest.open("get", server+"squarez/get_board", false);
	boardRequest.send();

	this.board = new Module.GameBoard(boardRequest.responseText);

	UserInterface.call(this, rootElement, scoreElement);

	//FIXME: get parameters from the server
	this.timer = new Module.Timer(10);
	this.updateTimer();

	// Start the transition polling mechanism
	this.queryTransition();

	this.roundSelectionScore = 0;
}

MultiplayerRules.prototype = Object.create(UserInterface.prototype);
MultiplayerRules.prototype.select =  function(el)
{
	var pos = this.getPosition(el);
	var selected = this.selection.addPoint(pos.x, pos.y);
	if (selected)
	{
		el.classList.add("selected");
		var transition = this.board.selectSquare(this.selection, false);
		if (transition.score > 0)
		{
			if (transition.score > this.roundSelectionScore)
			{
				this.roundSelectionScore = transition.score;
				
				// Fire animations
				this.clearAnimations();
				this.drawSelection(this.selection);

				// Game part
				// Tell the server about the selection
				var pushRequest = new XMLHttpRequest();
				pushRequest.open("get", this.server+"squarez/push_selection?selection="+this.selection.serialize());
				pushRequest.send()
			}

			this.clearSelection();
		}
		transition.delete();
	}
	else
	{
		el.classList.remove("selected");
	}
};
MultiplayerRules.prototype.onTimerEvent = function()
{
};

MultiplayerRules.prototype.queryTransition = function()
{
	// Process the result if we are doing a query
	if (this.transitionQuery)
	{
		if (this.transitionQuery.readyState != 4)
			return;
		if (this.transitionQuery.status == 200)
		{
			var transition = new Module.Transition(this.transitionQuery.responseText)
			this.clearSelection();
			this.roundSelectionScore = 0;

			// Fire animations
			this.clearAnimations();
			try{
				this.animateSelection(transition.selection);
			} catch(err){}
			this.animateTransition(transition);

			this.board.applyTransition(transition);
			transition.delete();
			this.timer.refill(200);
		}
	}

	var that = this;

	// Schedule a new query
	this.transitionQuery = new XMLHttpRequest();
	this.transitionQuery.open("get", this.server+"squarez/get_transition");
	this.transitionQuery.onreadystatechange = function() {that.queryTransition();};
	this.transitionQuery.send();
}
