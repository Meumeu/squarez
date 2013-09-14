function SinglePlayerRules(rootElement, scoreElement)
{
	this.board = new Module.GameBoard(8,3);

	UserInterface.call(this, rootElement, scoreElement, -2);

	var that = this;
	var pauseButtons = document.getElementsByClassName("jsPause");
	for (var i = 0 ; i < pauseButtons.length; i++)
	{
		if (this.root.ontouchstart === undefined)
			pauseButtons[i].onclick = function() {that.pause();};
		else
			pauseButtons[i].ontouchstart = function() {that.pause();};
	}

	this.timer = new Module.Timer(10, 60, 180);
	this.updateTimer();

	this.getHighScores(true);
}

SinglePlayerRules.prototype = Object.create(UserInterface.prototype);
SinglePlayerRules.prototype.select =  function(el)
{
	var pos = this.getPosition(el);
	var selected = this.selection.addPoint(pos.x, pos.y);
	if (selected)
	{
		el.classList.add("selected");
		var transition = this.board.selectSquare(this.selection, false);
		if (transition.score > 0)
		{
			// Fire animations
			this.clearAnimations();
			this.animateSelection(this.selection);
			this.clearSelection();
			this.animateTransition(transition);

			// Game part
			this.score += transition.score;
			this.scoreElement.innerHTML = this.score;
			this.timer.refill(2*transition.score);
			this.board.applyTransition(transition);
		}
		transition.delete();
	}
	else
	{
		el.classList.remove("selected");
	}
};

SinglePlayerRules.prototype.onTimerEvent = function()
{
	if (this.timer.percentageLeft(0) == 0)
	{
		clearInterval(this.timerFunc);
		this.timerFunc = null;
		this.gameOver();
	}
};

SinglePlayerRules.prototype.gameOver = function()
{
	if (this.gameEnded)
		return;
	this.gameEnded = true;

	this.root.getElementsByClassName("gameOver")[0].style.display = "";
	var scores = this.getHighScores();
	var that = this;
	if (this.score > 0 && (scores.length < 10 || this.score >= scores[scores.length - 1].score))
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

SinglePlayerRules.prototype.pause = function()
{
	if (this.timer.paused())
	{
		this.timer.unPause();
		this.root.getElementsByClassName("pause")[0].style.display = "none";
		this.updateTimer();
	}
	else
	{
		this.timer.pause();
		clearInterval(this.timerFunc);
		this.timerFunc = null;
		this.root.getElementsByClassName("pause")[0].style.display = "";
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
			n.innerHTML="<td>"+res[i].name+ "</td><td>"+ res[i].date.toLocaleDateString() + "</td><td>" + res[i].score + "</td>";
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
		if (this.score >= scores[i].score)
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
