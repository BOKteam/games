/**
 * Created by lys.
 * User: Liu Xinyi
 * Date: 14-7-10
 * Time: 上午11:28
 * Write the description in this section.
 */
goog.provide("doudizhu.helper.Card");

/*
 Card define:
 spade   1-13 [1S, 2S ... XS, JS, QS, KS]
 heart   1-13 [1H, 2H ... XH, JH, QH, KH]
 club    1-13 [1C, 2C ... XC, JC, QC, KC]
 diamond 1-13 [1D, 2D ... XD, JD, QD, KD]
 jokers       [BJ, RJ]
*/

var CardType = [ '00',
    '1D', '2D', '3D', '4D', '5D', '6D', '7D', '8D', '9D', 'XD', 'JD', 'QD', 'KD',
    '1C', '2C', '3C', '4C', '5C', '6C', '7C', '8C', '9C', 'XC', 'JC', 'QC', 'KC',
    '1H', '2H', '3H', '4H', '5H', '6H', '7H', '8H', '9H', 'XH', 'JH', 'QH', 'KH',
    '1S', '2S', '3S', '4S', '5S', '6S', '7S', '8S', '9S', 'XS', 'JS', 'QS', 'KS',
    'BJ', 'RJ'
];

var CardHelper = {
    /**
     * @param {string} type Should came from CardType.
     * @return {number}
     * */
    getCardSuitOrder: function(type) {
        var v = type[1];
        var p = 0;
        switch(v) {
            case '0':
                p = 0;
                break;
            case 'D':
                p = 1;
                break;
            case 'C':
                p = 2;
                break;
            case 'H':
                p = 3;
                break;
            case 'S':
                p = 4;
                break;
            case 'J':
                p = 100;
                break;
            default:
                BOK.error("CardHelper.getCardSuitOrder: Unknown card value ["+type+"]");
        }

        return p;
    },

    /**
     * @param {string} type Should came from CardType.
     * @return {number}
     * */
    getCardPoint: function(type) {
        var v = type[0];
        var p = parseInt(v);
        switch(v) {
            case '0':
                p = 0;
                break;
            case 'X':
                p = 10;
                break;
            case 'J':
                p = 11;
                break;
            case 'Q':
                p = 12;
                break;
            case 'K':
                p = 13;
                break;
            case '1':
                p = 14;
                break;
            case '2':
                p = 50;
                break;
            case 'B':
                p = 99;
                break;
            case 'R':
                p = 100;
                break;
            default:
                if(isNaN(p))
                    BOK.error("CardHelper.getCardPoint: Unknown card value ["+type+"]");
        }

        return p;
    },

    /**@param {string|Object} card
     * @param {Array} hand contains {string|Object}
     * @param {Function=} getterFunc (optional) The function to extract card value
     *     if the hand contains something other than string
     * */
    insertCardToHand: function (card, hand, getterFunc) {
        //add card to proper place
        var cardValue = getterFunc ? getterFunc(card) : card;
        var p = this.getCardPoint(cardValue);
        var s = this.getCardSuitOrder(cardValue);
        var insertIndex = 0;
        BOK.each(hand, function(cardInHand){
            var cardValue = getterFunc ? getterFunc(cardInHand) : cardInHand;
            var cp = this.getCardPoint(cardValue);
            var cs = this.getCardSuitOrder(cardValue);

            if(cp > p || (cp == p && cs > s))
                return BOK.BREAK;

            insertIndex++;
        }, this);

        hand.splice(insertIndex, 0, card);

        return hand;
    },

    /**
     * @param {Array|Object} cards contains {string}
     * @param {number=} startIndex
     * @param {number=} checkLength
     * */
    sameKindInCards: function (cards, startIndex, checkLength) {
        var checkingCards = checkLength ? cards.concat().splice(startIndex, checkLength) : cards;
        var isSameKind = true;
        var onThePoint = 0;
        BOK.each(checkingCards, function(card){
            var point = this.getCardPoint(card);

            if(!onThePoint)
                onThePoint = point;
            else if(onThePoint != point)
                isSameKind = false;
        }, this);

        return isSameKind;
    },

    /**
     * @param {Array} cards contains {string}, must be a sorted card array
     * */
    straightInCards: function (cards) {
        var isStraight = true;
        var currentPoint = 0;
        BOK.each(cards, function(card){
            var point = this.getCardPoint(card);

            if(!currentPoint)
                currentPoint = point;
            else if(++currentPoint != point)
                isStraight = false;
        }, this);

        return isStraight;
    }
};
