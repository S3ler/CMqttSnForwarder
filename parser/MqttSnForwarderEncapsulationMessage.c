//
// Created by SomeDude on 19.05.2019.
//

#include "MqttSnForwarderEncapsulationMessage.h"
#include <string.h>
#include <assert.h>

int32_t parse_forwarder_encapsulation_header(ParsedMqttSnHeader *h, const uint8_t *data, uint16_t data_len, int32_t *parsed_bytes);
int32_t parse_forwarder_encapsulation_ctrl(const uint8_t *src_pos, uint16_t src_len, int32_t *parsed_bytes, uint8_t *radius);
int32_t parse_forwarder_encapsulation_wireless_node_id(const uint8_t *src_pos, uint16_t src_len, int32_t *parsed_bytes,
                                                       device_address *wireless_node_id);
int32_t parse_forwarder_encapsulation_mqtt_sn_message(const uint8_t *src_pos, uint16_t src_len, int32_t *parsed_bytes,
                                                      uint16_t mqtt_sn_message_to_parse_length, uint8_t *mqtt_sn_message,
                                                      uint16_t *mqtt_sn_message_length, uint16_t mqtt_sn_message_max_length);
int32_t parse_forwarder_encapsulation_byte(uint8_t *radius, device_address *wireless_node_id, uint8_t *mqtt_sn_message,
                                           uint16_t *mqtt_sn_message_len, uint16_t max_mqtt_sn_message_len, const uint8_t *data,
                                           uint16_t data_len) {
    int32_t            parsed_bytes = 0;
    ParsedMqttSnHeader h            = { 0 };
    if ((parsed_bytes = parse_forwarder_encapsulation_header(&h, data, data_len, &parsed_bytes)) < 0) {
        return -1;
    }
    if ((parsed_bytes = parse_forwarder_encapsulation_ctrl(data + parsed_bytes, data_len, &parsed_bytes, radius)) < 0) {
        return -1;
    }
    if ((parsed_bytes = parse_forwarder_encapsulation_wireless_node_id(data + parsed_bytes, data_len, &parsed_bytes, wireless_node_id)) <
        0) {
        return -1;
    }
    // data_len - parsed_bytes is the encapsulated payload
    if ((parsed_bytes = parse_forwarder_encapsulation_mqtt_sn_message(data + parsed_bytes, data_len, &parsed_bytes, data_len - parsed_bytes,
                                                                      mqtt_sn_message, mqtt_sn_message_len, max_mqtt_sn_message_len)) < 0) {
        return -1;
    }
    /* old wrong way
  if ((parsed_bytes = parse_forwarder_encapsulation_mqtt_sn_message(data + parsed_bytes,
                                                                    data_len,
                                                                    &parsed_bytes,
                                                                    h.length - parsed_bytes,
                                                                    mqtt_sn_message,
                                                                    mqtt_sn_message_len,
                                                                    max_mqtt_sn_message_len)) < 0) {
    return -1;
  }
  */
    return parsed_bytes;
}

int32_t parse_forwarder_encapsulation_mqtt_sn_message(const uint8_t *src_pos, uint16_t src_len, int32_t *parsed_bytes,
                                                      uint16_t mqtt_sn_message_to_parse_length, uint8_t *mqtt_sn_message,
                                                      uint16_t *mqtt_sn_message_length, uint16_t mqtt_sn_message_max_length) {
    return parse_mqtt_sn_uint8_array_byte(src_pos, src_len, parsed_bytes, mqtt_sn_message_to_parse_length, mqtt_sn_message,
                                          mqtt_sn_message_length, mqtt_sn_message_max_length);
}
int32_t parse_forwarder_encapsulation_wireless_node_id(const uint8_t *src_pos, uint16_t src_len, int32_t *parsed_bytes,
                                                       device_address *wireless_node_id) {
    uint16_t wireless_node_id_length = 0;
    if (parse_mqtt_sn_device_address(src_pos, src_len, parsed_bytes, wireless_node_id, &wireless_node_id_length) < 0) {
        return -1;
    }
    if (wireless_node_id_length != MQTT_SN_MESSAGE_FORWARDER_ENCAPSULATION_WIRELESSNODEID_LENGTH) {
        return -1;
    }
    return (*parsed_bytes);
}
int parse_forwarder_encapsulation_header(ParsedMqttSnHeader *h, const uint8_t *data, uint16_t data_len, int32_t *parsed_bytes) {
    if (parse_header(h, ENCAPSULATED_MESSAGE, data, data_len, parsed_bytes) < 0) {
        return -1;
    }
    if (h->length < MQTT_SN_MESSAGE_FORWARDER_ENCAPSULATION_MIN_LENGTH) {
        return -1;
    }
    /*
  if (h->length > MQTT_SN_MESSAGE_FORWARDER_ENCAPSULATION_MAX_LENGTH) {
    return -1;
  }
  */
    return *parsed_bytes;
}
int32_t parse_forwarder_encapsulation_ctrl(const uint8_t *src_pos, uint16_t src_len, int32_t *parsed_bytes, uint8_t *radius) {
    if (parse_mqtt_sn_uint8_byte(src_pos, src_len, parsed_bytes, radius) < 0) {
        return -1;
    }
    if ((*radius) > MQTT_SN_MESSAGE_FORWARDER_ENCAPSULATION_CRTL_BROADCAST_RADIUS) {
        return -1;
    }
    return *parsed_bytes;
}

int32_t generate_forwarder_encapsulation_header(uint8_t *dst, uint16_t dst_len, int32_t *gen_bytes, uint16_t mqtt_sn_message_length);
int     generate_forwarder_encapsulation_ctrl(uint8_t *dst_pos, uint16_t dst_len, int32_t *gen_bytes, uint8_t radius);
int     generate_forwarder_encapsulation_wireless_node_id(uint8_t *dst_pos, uint16_t dst_len, int32_t *gen_bytes,
                                                          const device_address *wireless_node_id);
int generate_forwarder_encapsulation_mqtt_sn_message(uint8_t *dst_pos, uint16_t dst_len, int32_t *gen_bytes, const uint8_t *mqtt_sn_message,
                                                     uint16_t mqtt_sn_message_length);
int32_t generate_forwarder_encapsulation_byte(uint8_t *dst, uint16_t dst_len, uint8_t radius, const device_address *wireless_node_id,
                                              const uint8_t *mqtt_sn_message, uint16_t mqtt_sn_message_length) {
    int32_t gen_bytes = 0;
    if (generate_forwarder_encapsulation_header(dst, dst_len, &gen_bytes, mqtt_sn_message_length) < 0) {
        return -1;
    }
    if (generate_forwarder_encapsulation_ctrl(dst + gen_bytes, dst_len, &gen_bytes, radius) < 0) {
        return -1;
    }
    if (generate_forwarder_encapsulation_wireless_node_id(dst + gen_bytes, dst_len, &gen_bytes, wireless_node_id) < 0) {
        return -1;
    }
    if (generate_forwarder_encapsulation_mqtt_sn_message(dst + gen_bytes, dst_len, &gen_bytes, mqtt_sn_message, mqtt_sn_message_length) <
        0) {
        return -1;
    }
    return gen_bytes;
}

int32_t parse_forwarder_encapsulation(ParseMqttSnEncapsulationMessage *mqtt_sn_encapsulation_message, const uint8_t *data,
                                      uint16_t data_len) {
    return parse_forwarder_encapsulation_byte(&mqtt_sn_encapsulation_message->radius, &mqtt_sn_encapsulation_message->wireless_node_id,
                                              mqtt_sn_encapsulation_message->mqtt_sn_message,
                                              &mqtt_sn_encapsulation_message->mqtt_sn_message_len,
                                              sizeof(mqtt_sn_encapsulation_message->mqtt_sn_message), data, data_len);
}
int32_t parse_forwarder_encapsulation_message_header(ParsedMqttSnEncapsulationMessageHeader *mqtt_sn_encapsulation_message_header,
                                                     const uint8_t *data, uint16_t data_len) {
    int32_t            parsed_bytes = 0;
    ParsedMqttSnHeader h            = { 0 };
    if ((parsed_bytes = parse_forwarder_encapsulation_header(&h, data, data_len, &parsed_bytes)) < 0) {
        return -1;
    }
    if ((parsed_bytes = parse_forwarder_encapsulation_ctrl(data + parsed_bytes, data_len, &parsed_bytes,
                                                           &mqtt_sn_encapsulation_message_header->radius)) < 0) {
        return -1;
    }
    if ((parsed_bytes = parse_forwarder_encapsulation_wireless_node_id(data + parsed_bytes, data_len, &parsed_bytes,
                                                                       &mqtt_sn_encapsulation_message_header->wireless_node_id)) < 0) {
        return -1;
    }
    return parsed_bytes;
}
int generate_forwarder_encapsulation_mqtt_sn_message(uint8_t *dst_pos, uint16_t dst_len, int32_t *gen_bytes, const uint8_t *mqtt_sn_message,
                                                     uint16_t mqtt_sn_message_length) {
    return generate_mqtt_sn_uint8_array(dst_pos, dst_len, gen_bytes, mqtt_sn_message, mqtt_sn_message_length);
}
int generate_forwarder_encapsulation_wireless_node_id(uint8_t *dst_pos, uint16_t dst_len, int32_t *gen_bytes,
                                                      const device_address *wireless_node_id) {
    return generate_mqtt_sn_device_address(dst_pos, dst_len, gen_bytes, wireless_node_id, sizeof(*wireless_node_id));
}
int generate_forwarder_encapsulation_ctrl(uint8_t *dst_pos, uint16_t dst_len, int32_t *gen_bytes, uint8_t radius) {
    if (radius > MQTT_SN_MESSAGE_FORWARDER_ENCAPSULATION_CRTL_BROADCAST_RADIUS) {
        return -1;
    }
    return generate_mqtt_sn_uint8(dst_pos, dst_len, radius, gen_bytes);
}
int32_t generate_forwarder_encapsulation_header(uint8_t *dst, uint16_t dst_len, int32_t *gen_bytes, uint16_t mqtt_sn_message_length) {
    uint16_t message_length = MQTT_SN_MESSAGE_FORWARDER_ENCAPSULATION_MIN_LENGTH;
    if (MQTT_SN_MESSAGE_FORWARDER_ENCAPSULATION_MIN_LENGTH + mqtt_sn_message_length > UINT8_MAX) {
        if (MQTT_SN_MESSAGE_FORWARDER_ENCAPSULATION_WO_MSG_MAX_LENGTH + mqtt_sn_message_length > UINT16_MAX) {
            return -1;
        }
        // message_length = MQTT_SN_MESSAGE_FORWARDER_ENCAPSULATION_WO_MSG_MAX_LENGTH + mqtt_sn_message_length;
    } else {
        // message_length = MQTT_SN_MESSAGE_FORWARDER_ENCAPSULATION_MIN_LENGTH + mqtt_sn_message_length;
    }
    return generate_mqtt_sn_header(dst, dst_len, gen_bytes, message_length, ENCAPSULATED_MESSAGE);
}

int32_t parse_multiple_forwarder_encapsulation_headers_byte(uint8_t *radiuses, device_address *forwarder_addresses,
                                                            uint16_t *forwarders_len, uint16_t forwarders_max_len,
                                                            device_address *first_forwarder_address, device_address *wireless_node_id,
                                                            const uint8_t *data, uint16_t data_len) {
    assert(forwarders_max_len > 0);
    assert(forwarders_max_len < 8000);

    (*forwarders_len)    = 0;
    int32_t parsed_bytes = 0;

    // parse encapsulated message header until: no encapsulated message or forwarder_addresses_max_len+1
    for (uint16_t i = 0; i < forwarders_max_len + 1; i++) {
        ParsedMqttSnEncapsulationMessageHeader h;
        int32_t parse_rc = parse_forwarder_encapsulation_message_header(&h, data + parsed_bytes, data_len - parsed_bytes);
        if (parse_rc < 0) {
            break;
        }
        if (i == forwarders_max_len) {
            // more encapsulation message as we can handle
            return -1;
        }
        radiuses[(*forwarders_len)]              = h.radius;
        forwarder_addresses[(*forwarders_len)++] = h.wireless_node_id;
        parsed_bytes += parse_rc;
    }

    if ((*forwarders_len) == 0) {
        return -1;
    }

    // reverse forwarder_addresses order
    for (uint16_t i = 0; i < (*forwarders_len) / 2; i++) {
        device_address tmp_addr   = forwarder_addresses[(*forwarders_len) - 1 - i];
        uint8_t        tmp_radius = radiuses[(*forwarders_len) - 1 - i];

        forwarder_addresses[(*forwarders_len) - 1 - i] = forwarder_addresses[i];
        radiuses[(*forwarders_len) - 1 - i]            = radiuses[i];

        forwarder_addresses[i] = tmp_addr;
        radiuses[i]            = tmp_radius;
    }

    (*wireless_node_id)    = forwarder_addresses[0];
    forwarder_addresses[0] = (*first_forwarder_address);

    return parsed_bytes;
}
int32_t generate_multiple_forwarder_encapsulation_headers_byte(uint8_t *dst, uint16_t dst_len, const uint8_t *radiuses,
                                                               const device_address *forwarder_addresses, uint16_t forwarders_len,
                                                               const device_address *wireless_node_id, const uint8_t *mqtt_sn_message,
                                                               uint16_t mqtt_sn_message_length) {
    assert(forwarders_len > 0);
    if (forwarders_len == 0) {
        return -1;
    }

    uint16_t enc_mqtt_sn_message_lengths[forwarders_len];

    for (uint16_t i = 0; i < forwarders_len; i++) {
        int32_t        gen_bytes = 0;
        device_address forwarder_address;
        uint8_t        forwarder_radius;
        uint16_t       current_mqtt_sn_message_length;
        if (i == 0) {
            forwarder_address              = (*wireless_node_id);
            forwarder_radius               = radiuses[i];
            current_mqtt_sn_message_length = mqtt_sn_message_length;
        } else {
            forwarder_address              = forwarder_addresses[i];
            forwarder_radius               = radiuses[i];
            current_mqtt_sn_message_length = enc_mqtt_sn_message_lengths[i - 1];
        }
        if (generate_forwarder_encapsulation_header(dst, dst_len, &gen_bytes, current_mqtt_sn_message_length) < 0) {
            return -1;
        }
        if (generate_forwarder_encapsulation_ctrl(dst + gen_bytes, dst_len, &gen_bytes, forwarder_radius) < 0) {
            return -1;
        }
        if (generate_forwarder_encapsulation_wireless_node_id(dst + gen_bytes, dst_len, &gen_bytes, &forwarder_address) < 0) {
            return -1;
        }
        enc_mqtt_sn_message_lengths[i] = current_mqtt_sn_message_length + gen_bytes;
    }

    int32_t gen_bytes = 0;
    for (uint16_t i = 0; i < forwarders_len; i++) {
        uint16_t       forwarder_index = forwarders_len - 1 - i;
        device_address forwarder_address;
        uint8_t        forwarder_radius;
        uint16_t       current_mqtt_sn_message_length;
        if (forwarder_index == 0) {
            forwarder_address              = (*wireless_node_id);
            forwarder_radius               = radiuses[forwarder_index];
            current_mqtt_sn_message_length = mqtt_sn_message_length;
        } else {
            forwarder_address              = forwarder_addresses[forwarder_index];
            forwarder_radius               = radiuses[forwarder_index];
            current_mqtt_sn_message_length = enc_mqtt_sn_message_lengths[forwarder_index - 1];
        }

        if (generate_forwarder_encapsulation_header(dst + gen_bytes, dst_len, &gen_bytes, current_mqtt_sn_message_length) < 0) {
            return -1;
        }
        if (generate_forwarder_encapsulation_ctrl(dst + gen_bytes, dst_len, &gen_bytes, forwarder_radius) < 0) {
            return -1;
        }
        if (generate_forwarder_encapsulation_wireless_node_id(dst + gen_bytes, dst_len, &gen_bytes, &forwarder_address) < 0) {
            return -1;
        }
    }
    if (generate_forwarder_encapsulation_mqtt_sn_message(dst + gen_bytes, dst_len, &gen_bytes, mqtt_sn_message, mqtt_sn_message_length) <
        0) {
        return -1;
    }
    assert(gen_bytes == enc_mqtt_sn_message_lengths[forwarders_len - 1]);
    return gen_bytes;
}
