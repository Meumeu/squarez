function MultiplayerRulesUI(rootElement, scoreElement, server, playerName)
{
	x = new MultiplayerRules(rootElement, scoreElement, server, playerName);
	var r = Module.UI.implement(x);
	r.setRules(x.rules);
	return r;
}

function MultiplayerRules(rootElement, scoreElement, server, playerName)
{
	SquarezUI.call(this, rootElement, scoreElement, 0, new Module.MultiPlayerRules(server, playerName));
}

MultiplayerRules.prototype = Object.create(SquarezUI.prototype);
