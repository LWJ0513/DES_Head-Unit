#include "mainwindow.h"

#include <QApplication>
#include "spotify.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Spotify spotify(1, NULL);

    // QWebEngineView 생성
    QWebEngineView view;

    // Spotify 플레이리스트를 iframe으로 로드
    QString spotifyEmbed = R"(
        <iframe
            title="Spotify Embed: Recommendation Playlist"
            src="https://open.spotify.com/embed/playlist/1iE71fV3n6CCfUtgsSwK2m?utm_source=generator&theme=0"
            width="100%"
            height="100%"
            style="min-height: 360px;"
            frameborder="0"
            allow="autoplay; clipboard-write; encrypted-media; fullscreen; picture-in-picture"
            loading="lazy">
        </iframe>
    )";

    // HTML을 QWebEngineView에 설정
    view.setHtml(spotifyEmbed);
    view.resize(800, 600);
    // view.showFullScreen();  전체화면
    view.showMaximized();

    // MainWindow w;
    // w.show();
    return a.exec();
}
