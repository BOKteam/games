/**
 * Created by lys.
 * User: Liu Xinyi
 * Date: 14-7-9
 * Time: 下午5:51
 * Write the description in this section.
 */
goog.provide("doudizhu.features.maingame.v.components.Deck");

goog.require("doudizhu.features.maingame.v.components.Card");

doudizhu.features.maingame.v.components.Deck = Deck;

BOK.inherits(Deck, createjs.Container);
function Deck() {
    createjs.Container.call(this);

    /** @type {Array} contains {doudizhu.features.maingame.v.components.Card}*/
    this.cards_ = [];
}

Deck.prototype.init = function() {
    for(var i=1; i<=54; ++i) {
        var newCard = new doudizhu.features.maingame.v.components.Card(i);
        this.cards_.push(newCard);
        if(this.parent) {
            this.parent.addChild(newCard);
        } else {
            BOK.error("Deck.init: unable to add card to the parent node of deck.Deck init fail.");
            return;
        }
    }

    this.layout();
};

/**
 * @param {Array} cards contains {string}
 * @param {string} bankerCard
 * */
Deck.prototype.setup = function(cards, bankerCard) {
    if(cards.length == this.cards_.length) {
        var newArray = [];
        var upCard = null;

        BOK.each(cards, function(card){
            BOK.each(this.cards_, function(cCard, index){
                if(cCard.value == card){
                    newArray.push(cCard);

                    //show card if it's banker card
                    if(cCard.value == bankerCard) {
                        cCard.show();
                        upCard = cCard;
                    }

                    this.cards_.splice(index, 1);
                    return BOK.BREAK;
                }
            }, this);
        }, this);

        this.cards_ = newArray;
        this.layout();
        upCard.y -= 20;
    } else {
        BOK.error('Deck.setup() invalid initial deck:');
        BOK.trace(cards);
    }
};

/**
 * @return {number}
 * */
Deck.prototype.cardsInDeck = function() {
    return this.cards_.length;
};

/**
 * @return {Card}
 * */
Deck.prototype.getTopCard = function() {
    var card = this.cards_.pop();

    //split if last 3
    if(this.cards_.length == 3) {
        createjs.Tween.get(this.cards_[0]).wait(1000).to({x:this.x - 100}, 500, createjs.Ease.cubicOut);
        createjs.Tween.get(this.cards_[2]).wait(1000).to({x:this.x + 100}, 500, createjs.Ease.cubicOut);
    }

    return card;
};

/**@param {Array} cards contains {Card}
 * */
Deck.prototype.addCards = function(cards) {
    if(cards.length) {
        this.cards_ = this.cards_.concat(cards);
        BOK.each(cards, function(card,index){
            createjs.Tween.get(card).wait(index * 50).to({x:this.x, y:this.y}, 300, createjs.Ease.cubicOut).call(function(){card.hide();});
        }, this);
        setTimeout(Delegate.create(this, this.layout), 300 + cards.length * 50);
    }
};

Deck.prototype.layout = function() {
    BOK.each(this.cards_, function(card, index){
        card.x = this.x + index/3;
        card.y = this.y - index/3;
        card.parent.setChildIndex(card, card.parent.children.length-1);
    }, this);
};

