import { ModeToggle } from '@/components/ModeToggle';
import { IconLink } from '@/components/IconLink';

import GithubDarkUrl from '@/assets/github-mark-white.svg?url';
import GithubLightUrl from '@/assets/github-mark.svg?url';
import LinkedInDarkUrl from '@/assets/InBug-White.png?url';
import LinkedInLightUrl from '@/assets/InBug-Black.png?url';


export function Navbar() {
    return (
        <header className="sticky top-0 z-50 w-full bg-background/95 backdrop-blur supports-[backdrop-filter]:bg-background/60">
            <div className="container mx-auto flex h-14 items-center justify-between">
                <div className="mr-4 flex">
                    <a href="https://www.jimiwilson.tech/" className="flex items-center">
                        <span className="font-bold">Jimi Wilson</span>
                    </a>
                </div>
                <div className="flex items-center justify-end gap-2">
                    <IconLink
                        url="https://www.linkedin.com/in/jimi-wilson-6a0634203/"
                        text="LinkedIn"
                        mode="responsive"
                        lightIconUrl={LinkedInLightUrl}
                        darkIconUrl={LinkedInDarkUrl}
                        altText="LinkedIn Icon"
                    />
                    <IconLink
                        url="https://github.com/Jimi-Wilson"
                        text="Github"
                        mode="responsive"
                        lightIconUrl={GithubLightUrl}
                        darkIconUrl={GithubDarkUrl}
                        altText="Github Icon"
                    />
                    <ModeToggle />
                </div>
            </div>
        </header>
    );
}