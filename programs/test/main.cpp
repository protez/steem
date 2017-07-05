#include <appbase/application.hpp>
#include <steemit/plugins/http/http_plugin.hpp>
#include <steemit/plugins/test_api/test_api_plugin.hpp>
#include <steemit/plugins/api_register/api_register_plugin.hpp>
#include <steemit/plugins/chain/chain_plugin.hpp>
#include <steemit/plugins/p2p/p2p_plugin.hpp>

#include <fc/exception/exception.hpp>
#include <fc/thread/thread.hpp>
#include <fc/interprocess/signals.hpp>
#include <fc/log/logger.hpp>
#include <fc/log/logger_config.hpp>

#include <boost/exception/diagnostic_information.hpp>

#include <iostream>
#include <csignal>

using steemit::protocol::version;

int main( int argc, char** argv )
{
   try
   {
      #ifdef IS_TEST_NET
      std::cerr << "------------------------------------------------------\n\n";
      std::cerr << "            STARTING TEST NETWORK\n\n";
      std::cerr << "------------------------------------------------------\n";
      auto initminer_private_key = graphene::utilities::key_to_wif( STEEMIT_INIT_PRIVATE_KEY );
      std::cerr << "initminer public key: " << STEEMIT_INIT_PUBLIC_KEY_STR << "\n";
      std::cerr << "initminer private key: " << initminer_private_key << "\n";
      std::cerr << "chain id: " << std::string(STEEMIT_CHAIN_ID) << "\n";
      std::cerr << "blockchain version: " << fc::string( STEEMIT_BLOCKCHAIN_VERSION ) << "\n";
      std::cerr << "------------------------------------------------------\n";
#else
      std::cerr << "------------------------------------------------------\n\n";
      std::cerr << "            STARTING STEEM NETWORK\n\n";
      std::cerr << "------------------------------------------------------\n";
      std::cerr << "initminer public key: " << STEEMIT_INIT_PUBLIC_KEY_STR << "\n";
      std::cerr << "chain id: " << std::string(STEEMIT_CHAIN_ID) << "\n";
      std::cerr << "blockchain version: " << fc::string( STEEMIT_BLOCKCHAIN_VERSION ) << "\n";
      std::cerr << "------------------------------------------------------\n";
#endif

      appbase::app().register_plugin< steemit::plugins::http::http_plugin >();
      appbase::app().register_plugin< steemit::plugins::api_register::api_register_plugin >();
      appbase::app().register_plugin< steemit::plugins::test_api::test_api_plugin >();
      appbase::app().register_plugin< steemit::plugins::chain::chain_plugin >();
      appbase::app().register_plugin< steemit::plugins::p2p::p2p_plugin >();
      if( !appbase::app().initialize( argc, argv ) )
         return -1;
      appbase::app().startup();
      appbase::app().exec();
   }
   catch ( const boost::exception& e )
   {
      std::cerr << boost::diagnostic_information(e) << "\n";
   }
   catch ( const std::exception& e )
   {
      std::cerr << e.what() << "\n";
   }
   catch ( ... )
   {
      std::cerr << "unknown exception\n";
   }

   std::cout << "exited cleanly\n";
   return 0;
}