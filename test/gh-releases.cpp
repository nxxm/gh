
#include <gh/releases.hxx>
#include <cassert>
#include <ctime>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream> 
#include <pre/json/to_json.hpp>

#define assertm(exp, msg) assert(((void)msg, exp))

std::string get_raw_str_dummyzip() {
  // this is an zip with a helloworld.txt
  static uint8_t dummyzip[] = { 
    0x50, 0x4B, 0x03, 0x04, 0x14, 0x00, 0x00, 0x00, 0x00, 0x00, 0x6F, 0x56, 0x03, 0x55, 0xA3, 0x1C,
    0x29, 0x1C, 0x0C, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x00, 0x00, 0x0E, 0x00, 0x00, 0x00, 0x68, 0x65,
    0x6C, 0x6C, 0x6F, 0x77, 0x6F, 0x72, 0x6C, 0x64, 0x2E, 0x74, 0x78, 0x74, 0x48, 0x65, 0x6C, 0x6C,
    0x6F, 0x20, 0x57, 0x6F, 0x72, 0x6C, 0x64, 0x21, 0x50, 0x4B, 0x01, 0x02, 0x14, 0x00, 0x14, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x6F, 0x56, 0x03, 0x55, 0xA3, 0x1C, 0x29, 0x1C, 0x0C, 0x00, 0x00, 0x00,
    0x0C, 0x00, 0x00, 0x00, 0x0E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x20, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x68, 0x65, 0x6C, 0x6C, 0x6F, 0x77, 0x6F, 0x72, 0x6C, 0x64,
    0x2E, 0x74, 0x78, 0x74, 0x50, 0x4B, 0x05, 0x06, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x01, 0x00,
    0x3C, 0x00, 0x00, 0x00, 0x38, 0x00, 0x00, 0x00, 0x00, 0x00
  };

  return std::string(std::begin(dummyzip), std::end(dummyzip));
}

int main(int argc, char** argv) {

  gh::list_releases("tipi-build", "cli", [](std::vector<gh::releases::release_t>&& r) {
    assertm(r.size() > 10, "tipi-build/cli has more than 10 releases");

    // now get the first release by id...
    size_t id_to_fetch = r[0].id;
    gh::get_release_by_id("tipi-build", "cli", id_to_fetch, 
      [&id_to_fetch](gh::releases::release_t&& r_by_id) {
        assertm(r_by_id.id == id_to_fetch, "Testing gh::get_release_by_id(): IDs did not match unexpectedly");
      });

    // test get_release_asset_info
    size_t asset_id_to_fetch = r[0].assets[0].id;

    gh::get_release_asset_info("tipi-build", "cli", asset_id_to_fetch,
      [&asset_id_to_fetch](gh::releases::asset_t&& asset_by_id) {
        assertm(asset_by_id.id == asset_id_to_fetch, "Testing gh::get_release_asset_info(): IDs did not match unexpectedly");
      });

  });

  gh::get_latest_release("daminetreg", "trommeli", [](gh::releases::release_t&& r) {
    std::cout << pre::json::to_json(r).dump(2) << std::endl;
  });

  gh::get_release_by_tag("tipi-build","cli","v0.0.19", [](gh::releases::release_t&& r) {
    std::cout << pre::json::to_json(r).dump(2) << std::endl;
  });

  gh::auth auth{ 
    std::getenv("GH_USER"),
    std::getenv("GH_PASS")
  };

  gh::get_latest_release("tipi-build","cli", [](gh::releases::release_t&& r) {
    std::cout << pre::json::to_json(r).dump(2) << std::endl;
  }, auth);

  // testing the release creation
  {
    std::string repo_owner = std::getenv("GH_RELEASE_TEST_REPO_OWNER");
    std::string repo_name = std::getenv("GH_RELEASE_TEST_REPO_NAME");

    std::stringstream release_tag_ss; 
    release_tag_ss << "test-" << std::time(0);
    std::string release_tag = release_tag_ss.str();

    gh::releases::create_release_t release_data{
      .tag_name = release_tag, 
      .target_commitish = "main",
      .name = "A test release",
      .body = "This is a test release!",
      .draft = true,
      .prerelease = false,
      .generate_release_notes = false
    };

    gh::create_release("nxxm", "test-gh-client-release", release_data, 
      [&release_data, &release_tag, &auth](gh::releases::release_t&& r) {
        assertm(r.tag_name == release_data.tag_name, "Release tag-name shall be the as set");
        assertm(r.name == release_data.name, "Release name shall be the as set");
        assertm(r.draft == release_data.draft, "Release draft status shall be the as set");
        
        gh::releases::update_release_t release_update_data{
          .tag_name = release_tag, 
          .target_commitish = "main",
          .name = "A test release - updated",
          .body = "This is a test release!",
          .draft = false,
          .prerelease = true
        };

        gh::update_release("nxxm", "test-gh-client-release", r.id, release_update_data, 
          [&release_update_data, &auth](gh::releases::release_t&& r) {
            assertm(r.name == release_update_data.name, "Release name shall be the as set");
            assertm(r.prerelease == release_update_data.prerelease, "Release prerelease status shall be the as set");
            
            // now upload an empty zip \o/
            const std::string zip_content = get_raw_str_dummyzip();
            const std::string asset_name = "hello-world.zip";
            const size_t release_id = r.id;

            gh::create_release_asset("nxxm", "test-gh-client-release", r.id, 
              asset_name, "Hello world in a zip", zip_content, gh::releases::CONTENT_TYPE_ZIP, 
              [&asset_name, &zip_content, &release_id, &auth](gh::releases::asset_t new_asset) {
                assertm(asset_name == new_asset.name, "Test create_release_asset() : Asset names missmatch");
                assertm(zip_content.length() == new_asset.size, "Test create_release_asset() : Asset size/length missmatch");

                // update the label
                const std::string updated_label = "Hey thats new!!";
                gh::releases::update_asset_t asset_update{
                  .name = asset_name,
                  .label = updated_label
                };

                gh::update_release_asset("nxxm", "test-gh-client-release", new_asset.id, asset_update, 
                  [&asset_name, &zip_content, &updated_label, &release_id, &auth](gh::releases::asset_t updated_asset) {
                    assertm(asset_name == updated_asset.name, "Test update_release_asset() : Asset names missmatch");
                    assertm(updated_label == updated_asset.label, "Test update_release_asset() : Asset label missmatch");

                    // test downloading
                    gh::download_release_asset("nxxm", "test-gh-client-release", updated_asset.id, 
                      [&zip_content](std::string &&result) {
                        assertm(result == zip_content, "Test download_release_asset() : contents not equal");
                      }, 
                      auth);

                    // deleting stuff
                    gh::delete_release_asset("nxxm", "test-gh-client-release", updated_asset.id, auth);

                    gh::delete_release("nxxm", "test-gh-client-release", release_id, auth);
                  },
                  auth);
              },
              auth);

            //
          },
          auth);
      }, 
      auth);
  }

  return 0;
}
