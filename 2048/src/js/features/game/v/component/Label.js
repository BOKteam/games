/**
 * Created by Envee.
 *
 * Date: 14-10-23
 * Time: 上午10:51
 * @author: <a href="526597516@qq.com">luyc</a>
 * @version: 0.1
 */
goog.provide("tzfe.features.game.v.component.Label");
goog.require("org.createjs.easeljs.EaselJS");

tzfe.features.game.v.component.Label = Label;
BOK.inherits(Label, createjs.Container);

function Label(width, height, value, score) {
    createjs.Container.call(this);
    this.width = width;
    this.height = height;
    var text = new createjs.Text(value, "bold 12px ClearSans", "#FFFFFF").set({x: width / 2, y: height / 2, textAlign: 'center', textBaseline: 'bottom' });
    this.scoreText = new createjs.Text(score, "bold 14px ClearSans", "#FFFFFF").set({x: width / 2, y: height / 2, textAlign: 'center', textBaseline: 'top' });
    var labelBack = new createjs.Shape(new createjs.Graphics().beginFill('#BBADA0').drawRoundRect(0, 0, width, height, 2));

    this.scoreAction = new createjs.Text("+0", "bold 14px ClearSans", "#8F7A66").set({x: width / 2 - 5, y: height / 2, alpha: 0, textAlign: 'center', textBaseline: 'top'});

    this.addChild(labelBack, text, this.scoreText, this.scoreAction);
}

Label.prototype.updateScore = function (score, addScore) {
    if (addScore) {
        this.scoreAction.text = "+" + addScore;
        this.scoreAction.set({x: this.width / 2 - 5, y: this.height / 2, alpha: 1});
        createjs.Tween.removeTweens(this.scoreAction);
        createjs.Tween.get(this.scoreAction).to({alpha: 0, y: this.scoreAction.y - 40}, 1000);
    }
    this.scoreText.text = score;
};
