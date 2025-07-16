#include <iostream>
#include <mqtt_client_cpp.hpp>

int main() {
    boost::asio::io_context ioc;
    auto c = mqtt::make_client(ioc, "test.mosquitto.org", 1883);

    c->set_client_id("cid2");
    c->set_clean_session(true);
    c->set_keep_alive_sec(300);

    c->set_connack_handler(
        [&c](bool sp, mqtt::connect_return_code connack_return_code){
            std::cout << "Connack handler called" << std::endl;
            std::cout << "Session Present: " << std::boolalpha << sp << std::endl;
            std::cout << "Connack Return Code: " << connack_return_code << std::endl;
            if (connack_return_code == mqtt::connect_return_code::accepted) {
                c->subscribe("mqtt_cpp_topic1", mqtt::qos::exactly_once);
            }
            return true;
        }
    );
    c->set_close_handler(
        [] {
            std::cout << "closed" << std::endl;
        }
    );
    c->set_suback_handler(
        [&c](std::uint16_t packet_id, std::vector<mqtt::suback_return_code> results){
            std::cout << "suback received. packet_id: " << packet_id << std::endl;
            for (auto const& e : results) {
                std::cout << "subscribe result: " << e << std::endl;
            }
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
            std::cout << "contents: " << contents << std::endl;
            return true;
        }
    );

    c->connect();

    ioc.run();
}