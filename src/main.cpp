// The output goes now to the DAC pins GPIO25 (Channel 1) and GPIO26 (Channel 2).
// Channel 1 is left(Tip), channel 2 is right(Ring).

#include <Arduino.h>
#include "BluetoothA2DPSink.h"

BluetoothA2DPSink a2dp_sink;

// for esp_a2d_connection_state_t see https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/bluetooth/esp_a2dp.html#_CPPv426esp_a2d_connection_state_t
void connection_state_changed(esp_a2d_connection_state_t state, void *ptr){
  Serial.println(a2dp_sink.to_str(state));
}

// for esp_a2d_audio_state_t see https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/bluetooth/esp_a2dp.html#_CPPv421esp_a2d_audio_state_t
void audio_state_changed(esp_a2d_audio_state_t state, void *ptr){
  Serial.println(a2dp_sink.to_str(state));
}

void avrc_metadata_callback(uint8_t data1, const uint8_t *data2) {
  Serial.printf("AVRC metadata rsp: attribute id 0x%x, %s\n", data1, data2);
}

void setup() {

  Serial.begin(115200);
  while(!Serial);

  // i2s_config
  const i2s_config_t i2s_config = {
      .mode = (i2s_mode_t) (I2S_MODE_MASTER | I2S_MODE_TX | I2S_MODE_DAC_BUILT_IN),
      .sample_rate = 44100, // corrected by info from bluetooth
      .bits_per_sample = (i2s_bits_per_sample_t) 16, /* the DAC module will only take the 8bits from MSB */
      .channel_format =  I2S_CHANNEL_FMT_RIGHT_LEFT,
      .communication_format = (i2s_comm_format_t)I2S_COMM_FORMAT_STAND_MSB,
      .intr_alloc_flags = 0, // default interrupt priority
      .dma_buf_count = 8,
      .dma_buf_len = 64,
      .use_apll = false
  };
  a2dp_sink.set_i2s_config(i2s_config);

  a2dp_sink.set_on_connection_state_changed(connection_state_changed);
  a2dp_sink.set_on_audio_state_changed(audio_state_changed);

  a2dp_sink.set_avrc_metadata_attribute_mask(ESP_AVRC_MD_ATTR_TITLE | ESP_AVRC_MD_ATTR_PLAYING_TIME);
  a2dp_sink.set_avrc_metadata_callback(avrc_metadata_callback);

  a2dp_sink.start("InternalDAC");

}

void loop() {
}