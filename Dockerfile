FROM louisjenkinscs/atlas-hp

USER root

RUN apt-get -y update
RUN apt-get -y install libpmem-dev librpmem-dev libpmemblk-dev libpmemlog-dev libpmemobj-dev libpmempool-dev libpmempool-dev

WORKDIR /home/Atlas/pm-wrapper
COPY . .

RUN make atlas
RUN make pmdk

WORKDIR /home/Atlas/pm-wrapper/bin

ENTRYPOINT ["/bin/sh", "-c" , "./multiple_region_atlas && ./multiple_region_pmdk"]