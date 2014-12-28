/**
 * @author lys.BOK
 * Date: 14-2-9
 * Time: 下午9:46
 *
 * File over view.
 */

goog.provide("dotasim.features.maingame.MainGameFeatureNotes");
goog.require('bok.framework.core.FeatureNotesCollection');

var MainGameFeatureNotes = new FeatureNotesCollection('MainGameFeature');

///////////////////////////////////////INPUT NOTES///////////////////////////////////////////////////////////
/**
 * body {}: null
 * */
MainGameFeatureNotes.addInputNote('GAME_START');
/**
 * body {}: null
 * */
MainGameFeatureNotes.addInputNote('GAME_PAUSE');
/**
 * body {}: null
 * */
MainGameFeatureNotes.addInputNote('GAME_RESUME');

/**
 * body {Object}: {
 *     id: {String}         //hero ID
 *     lane: {Number}       //deployment lane
 * }
 * */
MainGameFeatureNotes.addInputNote('DEPLOY_HERO');
/**
 * body {String}: hero ID
 * */
MainGameFeatureNotes.addInputNote('CALL_BACK_HERO');

/**
 * Hero reference data will be put into the container.
 *
 * body {Object}: {
 *     data: {*}       //The container for hero data
 * }
 * */
MainGameFeatureNotes.addInputNote('REQUEST_HERO_DATA');
/**
 * Hero reference data will be put into the container.
 *
 * body {Object}: {
 *     data: {*}       //The container for player data
 * }
 * */
MainGameFeatureNotes.addInputNote('REQUEST_PLAYER_DATA');

///////////////////////////////////////OUT-GOING NOTES///////////////////////////////////////////////////////////
/**
 * body {}: null
 * */
MainGameFeatureNotes.addOutputNote('GAME_STARTED');
/**
 * body {}: null
 * */
MainGameFeatureNotes.addOutputNote('GAME_FINISHED');



///////////////////////////////////////INTERNAL NOTES///////////////////////////////////////////////////////////
/**
 * body {number}: gold amount
 * */
MainGameFeatureNotes.addInternalNote('PLAYER_GAIN_GOLD');
/**
 * body {}: null
 * */
MainGameFeatureNotes.addInternalNote('GAME_STAGE_UPDATE');

/**
 * body {Object}: Full Unit Data
 * */
MainGameFeatureNotes.addInternalNote('UNIT_KILLED');

/**
 * body {Object}: Full unit Data
 * */
MainGameFeatureNotes.addInternalNote('ADD_UNIT');

/**
 * body {String}: Hero ID
 * */
MainGameFeatureNotes.addInternalNote('FADE_AWAY_HERO');

/**
 * body {Object}:
 * {
 *     attackerID: {String},
 *     attackCD: {Number},
 *     targetID: {String}
 * }
 * */
MainGameFeatureNotes.addInternalNote('UNIT_ATTACK');


///////////////////////////////////////INTERNAL DEBUG NOTES///////////////////////////////////////////////////////////
MainGameFeatureNotes.addInternalNote('DEBUG_GRID_UPDATE');
