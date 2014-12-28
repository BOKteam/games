/**
 * @author lys.BOK
 * Date: 14-7-11
 * Time: 下午9:46
 *
 * File over view.
 */

goog.provide("doudizhu.features.core.CoreFeatureNotes");
goog.require('bok.framework.core.FeatureNotesCollection');

var CoreFeatureNotes = new FeatureNotesCollection('CoreFeature');

///////////////////////////////////////INPUT NOTES///////////////////////////////////////////////////////////
/**
 * body {null}
 * */
CoreFeatureNotes.addInputNote('READY_TO_START');

/**
 * body {Object}:
 * {
 *     name: {string}   //name of player should be one of 'banker', 'player1', 'player2'
 *     cards: {Array}   //array of card types played
 * }
 * */
CoreFeatureNotes.addInputNote('PLAY_HAND');

/**
 * body {null}: the pass command will notify game that the current play choose to pass
 * */
CoreFeatureNotes.addInputNote('PLAYER_PASSED');

/**
 * body {null}: notify game that the bid can start
 * */
CoreFeatureNotes.addInputNote('BID_READY_TO_START');

/**
 * body {Object}:
 *  {
 *      name: {string}      //name of bidder
 *      bid: {number}
 *  }
 * */
CoreFeatureNotes.addInputNote('BID_BANKER');

///////////////////////////////////////OUTPUT NOTES///////////////////////////////////////////////////////////
/**
 * body {Object}:
 * {
 *     name: {string}   //name of player should be one of 'banker', 'player1', 'player2'
 *     cards: {Array}   //array of card types played
 * }
 * */
CoreFeatureNotes.addOutputNote('HAND_PLAYED');

/**
 * body {Object}:
 * {
 *     name: {string}   //name of player should be one of 'banker', 'player1', 'player2'
 * }
 * */
CoreFeatureNotes.addOutputNote('PLAYER_PASSED');

/**
 * body {Object}:
 * {
 *     name: {string}   //name of player should be one of 'banker', 'player1', 'player2'
 *     allowPass: {boolean}
 *     suggestCards: {Array}    //an array of cards that suggest this player to play
 * }
 * */
CoreFeatureNotes.addOutputNote('NEXT_PLAYER');

/**
 * body {Object}:
 * {
 *     playerName: {string} //actual name of player
 *     deck: {Array}        //contains {string}
 *     bankerCard: {string} //the card that who takes should be banker.
 *     sideNames: {Array}  //name of each side of this game.
 * }
 * */
CoreFeatureNotes.addOutputNote('GAME_INIT');

/**
 * body {Object}:
 * {
 *     bankerName: {string}         //
 *     player1Name: {string}         //
 *     player2Name: {string}         //
 *     bottomCards: {Array}         //contains {string}
 * }
 * */
CoreFeatureNotes.addOutputNote('GAME_START');

/**
 * body {Object}:
 * {
 *     bankerWon: {boolean}
 *     point: {number}
 *     bonusFold: {number}
 * }
 * */
CoreFeatureNotes.addOutputNote('GAME_END');

/**
 * body {Object}:
 * {
 *     name: {string}
 *     bid: {number}     //The number of current bid
 *     lastCallerPassed: {boolean}
 * }
 * */
CoreFeatureNotes.addOutputNote('NEXT_CALLER');

/**
 * body {Object}:
 * {
 *     waitingNum: {number} //lobby waiting game player number
 *     readyDuration: {number} //ready connection time
 * }
 * */
CoreFeatureNotes.addOutputNote('NETWORK_WAITING');
