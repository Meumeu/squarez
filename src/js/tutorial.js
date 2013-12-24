function TutorialRulesUI(rootElement, scoreElement)
{
	x = new TutorialRules(rootElement, scoreElement);
	var r = Module.UI.implement(x);
	r.setRules(x.rules);

	return r;
}

function TutorialRules(rootElement, scoreElement)
{
	SquarezUI.call(this, rootElement, scoreElement, -2, new Module.TutorialRules(8,3));
}

TutorialRules.prototype = Object.create(SquarezUI.prototype);
