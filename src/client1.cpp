#include <iostream>
#include <mqtt_client_cpp.hpp>

int main() {
    boost::asio::io_context ioc;
    auto c = mqtt::make_sync_client(ioc, "test.mosquitto.org", 1883);

    c->set_client_id("cid1");
    c->set_clean_session(true);

    c->set_connack_handler(
        [&c](bool sp, mqtt::connect_return_code connack_return_code){
            std::cout << "Connack handler called" << std::endl;
            std::cout << "Session Present: " << std::boolalpha << sp << std::endl;
            std::cout << "Connack Return Code: " << connack_return_code << std::endl;
            if (connack_return_code == mqtt::connect_return_code::accepted) {
                c->subscribe("mqtt_cpp_topic1", mqtt::qos::at_most_once);
            }
            return true;
        }
    );
    c->set_close_handler(
        [] {
        }
    );
    c->set_error_handler(
        []
        (boost::system::error_code const& ec) {
        }
    );
    c->set_puback_handler(
        [](std::uint16_t packet_id) {
            return true;
        }
    );
    c->set_pubrec_handler(
        [](std::uint16_t packet_id) {
            return true;
        }
    );
    c->set_pubcomp_handler(
        [](std::uint16_t packet_id) {
            return true;
        }
    );
    c->set_suback_handler(
        [&c](std::uint16_t packet_id, std::vector<mqtt::suback_return_code> results){
            std::cout << "suback received. packet_id: " << packet_id << std::endl;
            for (auto const& e : results) {
                std::cout << "subscribe result: " << e << std::endl;
            }
            c->publish("mqtt_cpp_topic1", "ABC", mqtt::qos::at_most_once);
            return true;
        }
    );
    c->set_publish_handler(
        [&c]
        (mqtt::optional<std::uint16_t> packet_id,
         mqtt::publish_options pubopts,
         mqtt::buffer topic_name,
         mqtt::buffer contents) {
            std::cout << "publish received."
                      << " dup: "    << pubopts.get_dup()
                      << " qos: "    << pubopts.get_qos()
                      << " retain: " << pubopts.get_retain() << std::endl;
            if (packet_id)
                std::cout << "packet_id: " << *packet_id << std::endl;
            std::cout << "topic_name: " << topic_name << std::endl;
            std::cout << "test1: " << contents << std::endl;
            c->disconnect();
            return true;
        }
    );

    c->connect();

    ioc.run();
}
