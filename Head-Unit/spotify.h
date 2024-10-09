#ifndef SPOTIFY_H
#define SPOTIFY_H

#include <QWebEngineView>
#include <QUrl>
#include <QUrlQuery>
#include <QMessageBox>

class Spotify : public QWebEngineView {
    Q_OBJECT

public:
    Spotify(int a, QWidget* parent);

private:
    void getAccessToken();


private slots:
    void handleRedirect(const QUrl& url);
};


#endif // SPOTIFY_H
