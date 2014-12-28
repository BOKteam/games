/**
 * Created by Envee.
 *
 * Date: 14-10-30
 * Time: 下午2:40
 * @author: <a href="526597516@qq.com">luyc</a>
 * @version: 0.1
 */

appConfig = {
    // ajax url
    gameServer: {
        url: '/socket',
        port: 8081
    },
    // ajax url
    ajaxUrl: '/rest/',
    //rank server
    rankServer:'/rest/',
    gameType: 15,
    hudAPI: window.parent && window.parent.hudAPI,
    player: {id: Math.floor(Math.random() * 1000),name: 'Player' + Math.floor(Math.random() * 1000), seat: Math.floor(Math.random() * 100), avatar: "assets/img/avatar" + Math.floor(Math.random() * 10) + ".jpg"}
};