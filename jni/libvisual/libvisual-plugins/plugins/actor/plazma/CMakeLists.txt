INCLUDE_DIRECTORIES(
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_BINARY_DIR}
  ${LIBVISUAL_INCLUDE_DIRS}
)

LINK_DIRECTORIES(
  ${LIBVISUAL_LIBRARY_DIRS}
)

SET(actor_plazma_SOURCES
  actor_plazma.c
  plazma.c
)

ADD_LIBRARY(actor_plazma MODULE ${actor_plazma_SOURCES})
#-avoid-version

TARGET_LINK_LIBRARIES(actor_plazma
  ${LIBVISUAL_LIBRARIES}
)

INSTALL(TARGETS actor_plazma LIBRARY DESTINATION ${LV_ACTOR_PLUGIN_DIR})
