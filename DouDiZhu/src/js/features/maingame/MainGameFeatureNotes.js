/**
 * @author lys.BOK
 * Date: 14-7-11
 * Time: 下午9:46
 *
 * File over view.
 */

goog.provide("doudizhu.features.maingame.MainGameFeatureNotes");
goog.require('bok.framework.core.FeatureNotesCollection');

var MainGameFeatureNotes = new FeatureNotesCollection('MainGameFeature');

///////////////////////////////////////INPUT NOTES///////////////////////////////////////////////////////////
/**
 * body {Object}:
 * {
 *     name: {string}   //name of player should be one of 'banker', 'player1', 'player2'
 *     cards: {Array}   //array of card types played
 * }
 * */
MainGameFeatureNotes.addInputNote('PLAY_HAND');

///////////////////////////////////////OUTPUT NOTES///////////////////////////////////////////////////////////
/**
 * body {Object}:
 * {
 *     name: {string}   //name of player should be one of 'banker', 'player1', 'player2'
 *     cards: {Array}   //array of card types played
 * }
 * */
MainGameFeatureNotes.addOutputNote('HAND_SELECTED');

