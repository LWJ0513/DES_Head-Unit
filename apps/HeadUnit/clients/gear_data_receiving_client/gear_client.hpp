#ifndef GEAR_CLIENT_HPP
#define GEAR_CLIENT_HPP

#include <QObject>

#include "../headers.hpp"
#include "../server.hpp"

class GearClient : public QObejct
{
    Q_OBJECT
public:
    explicit GearClient(QObject *parent = nullptr);

    bool init();
    void start();
    void stop();

signals:
    void gearValueChanged(int newGearValue);

private:
    int gearValue;

    void on_state(vsomeip::state_type_e _state);
    void on_availability(vsomeip::service_t _service, vsomeip::instance_t _instance, bool _is_available);
    void on_message(const std::shared_ptr<vsomeip::message>& _response);

    std::shared_ptr<vsomeip::application> app_;
};

#endif // GEAR_CLIENT_HPP
