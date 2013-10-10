function SinglePlayerRulesUI(rootElement, scoreElement)
{
	x = new SinglePlayerRules(rootElement, scoreElement);
	var r = Module.UI.implement(x);
	r.setRules(x.rules);
	return r;
}

function SinglePlayerRules(rootElement, scoreElement)
{
	SquarezUI.call(this, rootElement, scoreElement, -2, new Module.SinglePlayerRules(8,3,10,60,180));
}

SinglePlayerRules.prototype = Object.create(SquarezUI.prototype);
