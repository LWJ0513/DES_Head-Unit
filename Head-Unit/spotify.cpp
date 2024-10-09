#include "spotify.h"

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QUrl>
#include <QDebug>


Spotify::Spotify(int a, QWidget* parent = nullptr) : QWebEngineView(parent)  {

    getAccessToken();

    // QString auth_url = QString("https://accounts.spotify.com/authorize?response_type=code&client_id=%1&redirect_uri=%2&scope=%3")
    //                        .arg(client_id).arg(redirect_uri).arg(scope);

    // Spotify 로그인 페이지 로드
    // load(QUrl(auth_url));

    // URL이 변경될 때 신호를 연결하여 리디렉션을 감지
    // connect(this, &QWebEngineView::urlChanged, this, &Spotify::handleRedirect);
}

void Spotify::getAccessToken() {
    /*
 * curl -X POST "https://accounts.spotify.com/api/token" \
     -H "Content-Type: application/x-www-form-urlencoded" \
     -d "grant_type=client_credentials&client_id=your-client-id&client_secret=your-client-secret"
 */

    QNetworkAccessManager *manager = new QNetworkAccessManager();
    QUrl url("https://accounts.spotify.com/api/token");
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    QUrlQuery postData;
    postData.addQueryItem("grant_type", "client_credentials");
    postData.addQueryItem("client_id", "");      // Spotify Client ID
    postData.addQueryItem("client_secret", "");  // Spotify Client Secret

    QNetworkReply *reply = manager->post(request, postData.toString(QUrl::FullyEncoded).toUtf8());

    QObject::connect(reply, &QNetworkReply::finished, [reply]() {
        if (reply->error() == QNetworkReply::NoError) {
            // 7. 서버 응답 처리 (JSON 데이터 읽기)
            QByteArray response_data = reply->readAll();
            QJsonDocument jsonResponse = QJsonDocument::fromJson(response_data);
            QJsonObject jsonObject = jsonResponse.object();
            qDebug() << "Access Token Response:" << jsonObject;

            // 액세스 토큰 확인 (응답 JSON의 "access_token" 필드)
            QString accessToken = jsonObject["access_token"].toString();
            qDebug() << "Access Token:" << accessToken;
        } else {
            // 에러 발생 시 처리
            qDebug() << "Error:" << reply->errorString();
        }

        // reply 객체 해제
        reply->deleteLater();
    });
}


void Spotify::handleRedirect(const QUrl& url) {
    // 리디렉션 URL에서 인증 코드를 추출
    if (url.toString().startsWith("YOUR_REDIRECT_URI")) {
        QUrlQuery query(url);
        if (query.hasQueryItem("code")) {
            QString auth_code = query.queryItemValue("code");

            // 인증 코드를 얻었다는 메시지 박스 (추후 토큰 요청 코드로 대체)
            QMessageBox::information(this, "Authorization Code", "Code: " + auth_code);

            // 인증 코드를 사용해 액세스 토큰을 요청하는 로직을 여기에 추가
            // 이후 필요한 API 호출 구현
        }
    }
}

void sendPostRequest() {
    // 1. QNetworkAccessManager 객체 생성
    QNetworkAccessManager *manager = new QNetworkAccessManager();

    // 2. 요청할 URL 설정
    QUrl url("https://jsonplaceholder.typicode.com/posts");  // 테스트용 API
    QNetworkRequest request(url);

    // 3. 헤더 설정 (특히 JSON을 보낼 때는 Content-Type 설정이 중요)
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    // 4. JSON 데이터 생성
    QJsonObject json;
    json["title"] = "foo";
    json["body"] = "bar";
    json["userId"] = 1;

    // QJsonDocument를 사용해 JSON 객체를 QByteArray로 변환
    QJsonDocument jsonDoc(json);
    QByteArray jsonData = jsonDoc.toJson();

    // 5. POST 요청 전송
    QNetworkReply *reply = manager->post(request, jsonData);

    // 6. 응답이 완료되었을 때 처리하는 슬롯 연결
    QObject::connect(reply, &QNetworkReply::finished, [reply]() {
        if (reply->error() == QNetworkReply::NoError) {
            // 7. 서버 응답 처리
            QByteArray response_data = reply->readAll();
            QJsonDocument jsonResponse = QJsonDocument::fromJson(response_data);

            // JSON 응답 데이터를 읽음
            QJsonObject jsonObject = jsonResponse.object();
            qDebug() << "Response JSON:" << jsonObject;

            // JSON 값을 출력
            qDebug() << "ID:" << jsonObject["id"].toInt();
        } else {
            // 에러 처리
            qDebug() << "Error:" << reply->errorString();
        }

        // reply 객체 해제
        reply->deleteLater();
    });
}
