/**
 * Created by Envee.
 *
 * Date: 14-10-21
 * Time: 上午9:06
 * @author: <a href="526597516@qq.com">luyc</a>
 * @version: 0.1
 */

goog.provide("quiz.features.game.MainGameFeature");
goog.require("bok.framework.core.MVCFeature");
goog.require("quiz.features.game.v.GameViewMediator");
goog.require("quiz.features.game.v.GamePlayMediator");
goog.require("quiz.features.game.v.TopicsViewMediator");
goog.require("quiz.features.game.v.MainViewMediator");
goog.require("quiz.features.game.v.QuizResultViewMediator");
goog.require("quiz.features.game.v.MatchSuccessViewMediator");
goog.require("quiz.features.game.v.HistoryViewMediator");
goog.require("quiz.features.game.v.RematchViewMediator");

quiz.features.game.MainGameFeature = MainGameFeature;
BOK.inherits(MainGameFeature, MVCFeature);
function MainGameFeature(stage) {
    MVCFeature.call(this);

    this.addMediator(new quiz.features.game.v.GameViewMediator(stage));
    this.addMediator(new quiz.features.game.v.TopicsViewMediator(stage));
    this.addMediator(new quiz.features.game.v.RematchViewMediator(stage));
    this.addMediator(new quiz.features.game.v.MainViewMediator(stage));
    this.addMediator(new quiz.features.game.v.MatchSuccessViewMediator(stage));
    this.addMediator(new quiz.features.game.v.QuizResultViewMediator(stage));
  	this.addMediator(new quiz.features.game.v.GamePlayMediator());
  	this.addMediator(new quiz.features.game.v.HistoryViewMediator(stage));
}
