/**
 * Created by lys.
 * User: Liu Xinyi
 * Date: 14-7-29
 * Time: 下午5:03
 * Write the description in this section.
 */
goog.provide("doudizhu.features.ui.GameUIFeatureNotes");
goog.require('bok.framework.core.FeatureNotesCollection');

var GameUIFeatureNotes = new FeatureNotesCollection('GameUIFeature');

///////////////////////////////////////OUTPUT NOTES///////////////////////////////////////////////////////////
/**
 * body {null}:
 * */
GameUIFeatureNotes.addOutputNote('BACK_TO_MENU');

/**
 * body {Object}:
 * {
 *     name: {string}   //name of player should be one of 'banker', 'player1', 'player2'
 *     cards: {Array}   //array of card types played
 * }
 * */
GameUIFeatureNotes.addOutputNote('PLAY_HINT');


