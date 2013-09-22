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

	this.getHighScores(true);
}

SinglePlayerRules.prototype = Object.create(SquarezUI.prototype);

SinglePlayerRules.prototype.gameOver = function()
{
	if (this.gameEnded)
		return;
	this.gameEnded = true;

	this.root.getElementsByClassName("gameOver")[0].style.display = "";
	var scores = this.getHighScores();
	var that = this;
	if (this.rules.score > 0 && (scores.length < 10 || this.rules.score >= scores[scores.length - 1].score))
	{
		var form = document.getElementById("nameForm");
		form.style.display = "";
		if (typeof(Storage) !== "undefined" && sessionStorage["lastname"])
			document.getElementById("nameInput").value = sessionStorage["lastname"];
		form.onsubmit = function()
		{
			var name = document.getElementById("nameInput").value;
			if (! name)
				return false;
			form.style.display = "none";
			sessionStorage["lastname"] = name;
			that.saveScore(name);
			that.getHighScores(true);
			return false;
		}
	}
};

SinglePlayerRules.prototype.getHighScores = function(draw)
{
	var res = new Array();
	if (typeof(Storage) === "undefined")
		return res;
	for (var i = 0 ; localStorage["score_val_"+i]; i++)
	{
		res.push({
			score: parseInt(localStorage["score_val_"+i]),
					date: new Date(parseInt(localStorage["score_date_"+i])),
					name: localStorage["score_name_"+i]
		});
	}
	if (draw)
	{
		var scoreList = document.getElementById("scoreList");
		scoreList.innerHTML = "";
		for (var i = 0 ; i < res.length ; i++)
		{
			document.getElementById("highScores").style.display="";
			var n = document.createElement("tr");
			n.innerHTML='<td class="name">'+res[i].name+ '</td><td class="date">'+ res[i].date.toLocaleDateString() + '</td><td class="score">' + res[i].score + "</td>";
			scoreList.appendChild(n);
		}
	}
	return res;
};

SinglePlayerRules.prototype.saveScore = function(name)
{
	if (typeof(Storage) === "undefined" || this.score == 0)
		return false;
	var scores = this.getHighScores();
	for (var i = 0 ; i < scores.length ; i++)
	{
		if (this.rules.score >= scores[i].score)
		{
			// Shift existing scores
			for (var j = Math.min(scores.length, 9); j > i ; j--)
			{
				localStorage["score_val_"+j] = localStorage["score_val_"+(j-1)];
				localStorage["score_date_"+j] = localStorage["score_date_"+(j-1)];
				localStorage["score_name_"+j] = localStorage["score_name_"+(j-1)]
			}
			break;
		}
	}
	if (i < 9)
	{
		localStorage["score_val_"+i] = this.score;
		localStorage["score_date_"+i] = Date.now();
		localStorage["score_name_"+i] = name;
		return true;
	}
	return false;
};
