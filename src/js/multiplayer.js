function MultiplayerRulesUI(rootElement, scoreElement, server, playerName)
{
	x = new MultiplayerRules(rootElement, scoreElement, server, playerName);
	var r = Module.UI.implement(x);
	r.setRules(x.rules);

	var grad = document.getElementById("timerScale");
	for (var i = 0 ; i < x.rules.numberOfRounds ; i++)
	{
		var n = document.createElement("div");
		n.style.width = (100/x.rules.numberOfRounds)+"%";
		n.style.height = n.style.width;
		grad.appendChild(n);
	}

	return r;
}

function MultiplayerRules(rootElement, scoreElement, server, playerName)
{
	SquarezUI.call(this, rootElement, scoreElement, 0, new Module.MultiPlayerRules(server, playerName));
}

MultiplayerRules.prototype = Object.create(SquarezUI.prototype);

MultiplayerRules.prototype.toGlobalTime = function(percent) { return (this.rules.currentRound + percent - 1) / this.rules.numberOfRounds};